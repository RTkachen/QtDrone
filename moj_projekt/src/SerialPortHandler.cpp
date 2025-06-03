#include "SerialPortHandler.hpp"
#include <QDateTime>
#include <QDebug>

/**
 * @brief Konstruktor klasy SerialPortHandler.
 *
 * Inicjalizuje obiekt i łączy sygnał odczytu danych z portu szeregowego
 * z odpowiednim slotem przetwarzającym dane.
 *
 * @param parent Wskaźnik na obiekt rodzica (domyślnie nullptr).
 */
SerialPortHandler::SerialPortHandler(QObject* parent)
    : QObject(parent)
{
    connect(&m_port, &QSerialPort::readyRead,
            this,      &SerialPortHandler::onReadyRead);
}

/**
 * @brief Oblicza 8-bitowy CRC (Polynomial 0x07) dla linii danych i zwraca go jako 2-znakowy HEX.
 *
 * Ta funkcja wykonuje klasyczny algorytm CRC-8:
 * 1. Inicjalizuje rejestr CRC wartością 0x00.
 * 2. Dla każdego bajtu z wejściowego @a dataLine wykonuje:
 *    - XOR rejestru CRC z aktualnym bajtem.
 *    - 8-krotnie:
 *      - Jeśli najstarszy bit (bit 7) rejestru CRC jest ustawiony:
 *          - przesuwa cały rejestr o 1 w lewo i XORuje go z wielomianem 0x07
 *      - w przeciwnym razie:
 *          - przesuwa rejestr o 1 w lewo
 * 3. Wynikowy bajt CRC po wszystkich iteracjach zamienia na ciąg 2 znaków heksadecymalnych (0–9, A–F).
 *
 * @param dataLine   Bufor danych, nad którym ma być policzony CRC-8.
 * @return           Dwie wielkie cyfry HEX, reprezentujące obliczony CRC-8
 *                   (np. `"3A"`, `"00"`, `"FF"`).
 */
QByteArray SerialPortHandler::computeCRC(const QByteArray& dataLine)
{
    uint8_t crc = 0x00;
    const uint8_t poly = 0x07;

    for (auto b : dataLine) {
        crc ^= static_cast<uint8_t>(b);
        for (int bit = 0; bit < 8; ++bit) {
            if (crc & 0x80) {
                crc = static_cast<uint8_t>((crc << 1) ^ poly);
            } else {
                crc <<= 1;
            }
        }
    }

    QByteArray hex = QByteArray::number(crc, 16).toUpper();
    if (hex.size() == 1)
        hex.prepend('0');
    return hex;
}

/**
 * @brief Zwraca listę dostępnych portów szeregowych.
 *
 * @return QStringList Lista nazw dostępnych portów.
 */
QStringList SerialPortHandler::availablePorts() const
{
    QStringList list;
    for (const auto& info : QSerialPortInfo::availablePorts())
        list << info.portName();
    return list;
}

/**
 * @brief Nawiązuje lub zrywa połączenie z portem szeregowym.
 *
 * Jeśli port jest już otwarty, zostaje zamknięty. W przeciwnym wypadku
 * zostaje otwarty w trybie tylko do odczytu z odpowiednimi parametrami transmisji.
 *
 * @param portName Nazwa portu szeregowego (np. COM3, ttyUSB0).
 */
void SerialPortHandler::toggleConnection(const QString& portName)
{
    if (m_port.isOpen()) {
        m_port.close();
        m_reading = false;
        return;
    }

    m_port.setPortName(portName);
    if (!m_port.open(QIODevice::ReadOnly)) {
        qCritical() << "Failed to open port:" << portName;
        return;
    }

    m_port.setBaudRate(QSerialPort::Baud115200);
    m_port.setDataBits(QSerialPort::Data8);
    m_port.setParity(QSerialPort::NoParity);
    m_port.setStopBits(QSerialPort::OneStop);
    m_port.setFlowControl(QSerialPort::NoFlowControl);
}

/**
 * @brief Rozpoczyna lub zatrzymuje odczyt danych z portu szeregowego.
 *
 * Jeśli port nie jest otwarty, zgłaszany jest błąd. W przeciwnym wypadku
 * stan odczytu jest przełączany, a bufor odczytu czyszczony.
 */
void SerialPortHandler::toggleReading()
{
    if (!m_port.isOpen()) {
        qCritical() << "toggleReading(): Port not open!";
        return;
    }

    m_reading = !m_reading;
    m_buffer.clear();
}

/**
 * @brief Slot wywoływany automatycznie, gdy dostępne są nowe dane z portu szeregowego.
 *
 * Przetwarza dane w formacie: A <x> <y> <z> <crc>. Weryfikuje poprawność CRC,
 * przelicza wartości na jednostki g i emituje sygnał z nowymi danymi.
 */
void SerialPortHandler::onReadyRead()
{
    if (!m_reading) return;

    m_buffer.append(m_port.readAll());
    int idx;
    while ((idx = m_buffer.indexOf('\r')) != -1) {
        QByteArray frame = m_buffer.left(idx).trimmed();
        m_buffer.remove(0, idx + 1);

        auto parts = frame.split(' ');
        if (parts.size() == 5 && parts[0] == "A") {
            QByteArray dataLine = parts.mid(0,4).join(' ') + ' ';
            QByteArray recvCRC  = parts[4];
            QByteArray calcCRC  = computeCRC(dataLine);

            if (recvCRC != calcCRC) {
                qCritical() << "CRC mismatch! recv=" << recvCRC
                            << " calc=" << calcCRC
                            << " frame=" << frame;
                continue;
            }

            bool ok1, ok2, ok3;
            int rawX = parts[1].toInt(&ok1);
            int rawY = parts[2].toInt(&ok2);
            int rawZ = parts[3].toInt(&ok3);

            if (ok1 && ok2 && ok3) {
                const double scale = 16382.0;
                double x_g = rawX / scale;
                double y_g = rawY / scale;
                double z_g = rawZ / scale;

                if (m_firstMeasurement) {
                    m_lastX = x_g;
                    m_lastY = y_g;
                    m_lastZ = z_g;
                    m_firstMeasurement = false;
                } else {
                    if (fabs(x_g - m_lastX) < 0.01)
                        x_g = m_lastX;
                    if (fabs(y_g - m_lastY) < 0.01)
                        y_g = m_lastY;
                    if (fabs(z_g - m_lastZ) < 0.01)
                        z_g = m_lastZ;
                }

                m_lastX = x_g;
                m_lastY = y_g;
                m_lastZ = z_g;
                emit newData(x_g, y_g, z_g);
            } else {
                qCritical() << "Parse error in frame (raw ints):" << frame;
            }
        } else {
            qCritical() << "Bad frame format:" << frame;
        }
    }
}

/**
 * @brief Aktualizuje interfejs użytkownika związany z połączeniem szeregowym.
 *
 * Metoda ta przełącza stan połączenia poprzez wywołanie toggleConnection z przekazanym portem.
 * Następnie sprawdza, czy połączenie zostało pomyślnie ustanowione:
 * - W przypadku aktywnego połączenia, przycisk Connect zmieniany jest na "Disconnect",
 *   a przycisk Start zostaje włączony.
 * - W przeciwnym razie, przycisk Connect przyjmuje etykietę "Connect", przycisk Start zostaje
 *   zresetowany do etykiety "Start", ustawiony na zielony kolor tła (biały tekst) i wyłączony.
 *
 * @param portName Nazwa portu, który ma być użyty do połączenia/rozłączenia.
 * @param btnConnect Wskaźnik do przycisku odpowiedzialnego za obsługę połączenia.
 * @param btnStart Wskaźnik do przycisku odpowiedzialnego za uruchomienie odczytu danych.
 */
void SerialPortHandler::updateConnectionUI(const QString& portName, QPushButton* btnConnect, QPushButton* btnStart)
{
    // Wyłączamy lub łączymy port wykorzystując przekazany portName.
    toggleConnection(portName);

    if (isConnected()) {
        btnConnect->setText(QObject::tr("Disconnect"));
        btnStart->setEnabled(true);
    } else {
        btnConnect->setText(QObject::tr("Connect"));
        btnStart->setText(QObject::tr("Start"));
        btnStart->setStyleSheet("background-color: green; color: white;");
        btnStart->setEnabled(false);
    }
}

/**
 * @brief Przełącza tryb odczytu danych i aktualizuje interfejs przycisku Start.
 *
 * Metoda ta wywołuje toggleReading(), która zmienia stan trybu odczytu.
 * Następnie na podstawie stanu odczytu:
 * - Jeśli odczyt jest aktywny, przycisk Start otrzymuje etykietę "Stop".
 * - Jeśli odczyt zostaje wyłączony, przycisk Start wraca do etykiety "Start", a jego styl zostaje ustawiony na zielony z białym tekstem.
 *
 * @param btnStart Wskaźnik do przycisku Start, który służy do uruchamiania i zatrzymywania odczytu danych.
 */
void SerialPortHandler::toggleReadingUI(QPushButton* btnStart)
{
    // Przełączamy tryb odczytu
    toggleReading();

    if (isReading()) {
        btnStart->setText(QObject::tr("Stop"));
    } else {
        btnStart->setText(QObject::tr("Start"));
        btnStart->setStyleSheet("background-color: green; color: white;");
    }
}

/**
 * @brief Odświeża listę dostępnych portów szeregowych w podanym QComboBox.
 *
 * Metoda ta najpierw czyści zawartość przekazanego combo boxa, a następnie
 * dodaje aktualnie dostępne porty, pobierając je poprzez wywołanie availablePorts().
 *
 * @param combo Wskaźnik do QComboBox, który ma być uaktualniony.
 */
void SerialPortHandler::refreshAvailablePorts(QComboBox* combo)
{
    combo->clear();
    combo->addItems(availablePorts());
}
