/**
 * @file SerialPortHandler.hpp
 * @brief Obsługa portu szeregowego oraz przetwarzania danych z akcelerometru.
 *
 * Klasa SerialPortHandler odpowiada za komunikację z portem szeregowym, odbieranie
 * i parsowanie danych (x, y, z) z akcelerometru oraz ich przesyłanie dalej przez sygnały Qt.
 * Obsługuje również wykrywanie dostępnych portów i obliczanie sumy kontrolnej CRC.
 */

#ifndef SERIALPORTHANDLER_HPP
#define SERIALPORTHANDLER_HPP

#include <QObject>
#include <QSerialPort>
#include <QSerialPortInfo>
#include <QPushButton>
#include <QComboBox>

/**
 * @class SerialPortHandler
 * @brief Klasa do obsługi połączenia szeregowego i odbioru danych akcelerometru.
 */
class SerialPortHandler : public QObject
{
    Q_OBJECT

public:
    /**
     * @brief Konstruktor klasy SerialPortHandler.
     * @param parent Obiekt nadrzędny (domyślnie nullptr).
     */
    explicit SerialPortHandler(QObject* parent = nullptr);

    /**
     * @brief Zwraca listę dostępnych portów szeregowych.
     * @return Lista nazw portów.
     */
    QStringList availablePorts() const;

    /**
     * @brief Przełącza połączenie z podanym portem szeregowym.
     * @param portName Nazwa portu, np. "COM3" lub "/dev/ttyUSB0".
     */
    void toggleConnection(const QString& portName);

    /**
     * @brief Włącza lub wyłącza tryb odczytu danych z portu.
     */
    void toggleReading();

    /**
 * @brief Sprawdza, czy połączenie z portem szeregowym jest aktywne.
 *
 * @retval true - jeśli port jest otwarty.
 * @retval false - jeśli port nie jest otwarty.
 */
    bool isConnected() const { return m_port.isOpen(); }

    /**
 * @brief Sprawdza, czy aktywny jest tryb odczytu danych.
 *
 * @retval true - jeśli dane są obecnie odczytywane.
 * @retval false -  jeśli tryb odczytu jest wyłączony.
 */
    bool isReading() const { return m_reading; }

    /**
    * @brief Oblicza 8-bitowy CRC (Polynomial 0x07) dla linii danych i zwraca go jako 2-znakowy HEX.
    * @param dataLine   Bufor danych, nad którym ma być policzony CRC-8.
    * @return           Dwie wielkie cyfry HEX, reprezentujące obliczony CRC-8
    */
    static QByteArray computeCRC(const QByteArray& dataLine);

    /**
     * @brief Aktualizuje interfejs użytkownika w związku z połączeniem z portem szeregowym.
     *
     * Metoda ta wywołuje funkcję toggleConnection, aby nawiązać lub rozłączyć połączenie według przekazanego
     * portu. Następnie na podstawie stanu połączenia aktualizuje tekst przycisków:
     * - Jeśli połączenie jest aktywne, ustawia przycisk Connect na "Disconnect" oraz włącza przycisk Start.
     * - W przeciwnym wypadku ustawia przycisk Connect na "Connect", przycisk Start na "Start", modyfikuje jego wygląd
     *   (zielone tło, biały tekst) i dezaktywuje go.
     *
     * @param portName Nazwa portu, który ma być użyty do połączenia/rozłączenia.
     * @param btnConnect Wskaźnik do przycisku odpowiedzialnego za łączenie/rozłączanie.
     * @param btnStart Wskaźnik do przycisku odpowiedzialnego za rozruch odczytu danych.
     */
    void updateConnectionUI(const QString& portName, QPushButton* btnConnect, QPushButton* btnStart);

    /**
     * @brief Przełącza tryb odczytu danych i aktualizuje wygląd przycisku Start.
     *
     * Metoda ta wywołuje funkcję toggleReading, aby zmienić tryb odczytu.
     * Po przełączeniu czytania:
     * - Jeśli tryb odczytu jest aktywny, przycisk Start zmienia tekst na "Stop".
     * - Jeśli tryb odczytu jest wyłączony, przycisk Start wraca do ustawień domyślnych ("Start" oraz zielony wygląd).
     *
     * @param btnStart Wskaźnik do przycisku Start, który wskazuje stan odczytu danych.
     */
    void toggleReadingUI(QPushButton* btnStart);

    /**
     * @brief Odświeża listę dostępnych portów szeregowych w podanym komponencie QComboBox.
     *
     * Metoda ta czyści zawartość podanego QComboBox i dodaje do niego aktualną listę dostępnych portów
     * pobraną przez metodę availablePorts().
     *
     * @param combo Wskaźnik do QComboBox, który ma być zaktualizowany.
     */
    void refreshAvailablePorts(QComboBox* combo);

signals:
    /**
     * @brief Sygnał emitowany po odebraniu nowych danych z akcelerometru.
     * @param x Wartość przyspieszenia w osi OX.
     * @param y Wartość przyspieszenia w osi OY.
     * @param z Wartość przyspieszenia w osi OZ.
     */
    void newData(double x, double y, double z);

private slots:
    /**
     * @brief Slot wywoływany, gdy port szeregowy odbierze nowe dane.
     */
    void onReadyRead();

private:
    double m_lastX = 0.0; ///< Ostatnia odebrana wartość osi OX
    double m_lastY = 0.0; ///< Ostatnia odebrana wartość osi OY
    double m_lastZ = 0.0; ///< Ostatnia odebrana wartość osi OZ
    bool m_firstMeasurement = true; ///< Flaga pierwszego pomiaru (pomijanie błędnych danych)
    QSerialPort m_port; ///< Obiekt portu szeregowego
    QByteArray m_buffer; ///< Bufor dla odebranych bajtów
    bool m_reading = false; ///< Flaga trybu odczytu danych
};

#endif // SERIALPORTHANDLER_HPP
