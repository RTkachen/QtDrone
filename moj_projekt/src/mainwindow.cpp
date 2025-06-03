/**
 * @file mainwindow.cpp
 * @brief Implementacja klasy MainWindow odpowiadającej za główne okno aplikacji.
 */

#include "mainwindow.hpp"
#include "ui/ui_mainwindow.h"
#include <QDebug>
#include <QSerialPortInfo>
#include <QList>
#include <QComboBox>
#include <QDateTime>

/**
 * @brief Konstruktor klasy MainWindow.
 * Inicjalizuje interfejs użytkownika, komponenty oraz połączenia sygnałów i slotów.
 * @param parent Wskaźnik na obiekt rodzica (domyślnie nullptr).
 */
MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ui->pushButtonStart->setStyleSheet("background-color: green; color: white;");

    // Inicjalizacja klasy Translator – odpowiada za zmianę języka interfejsu.
    translator = new Translator(ui->pushButtonLanguage, this);

    // Inicjalizacja kontrolera wyświetlaczy wartości oraz obsługi portu szeregowego.
    spinController = new SpinBoxController(ui->NumberX, ui->NumberY, ui->NumberZ, 0.01, -1.23, 3.14);
    serialHandler = new SerialPortHandler(this);

    // Aktualizacja listy dostępnych portów
    ui->comboBoxDevices->clear();
    ui->comboBoxDevices->addItems(serialHandler->availablePorts());

    // Łączenie sygnału, który po otrzymaniu danych aktualizuje wartości w kontrolerze spinboxów.
    connect(serialHandler, &SerialPortHandler::newData,
            spinController, &SpinBoxController::updateAll);

    // Aktualizacja widgetu 3D (openGLWidget) w zależności od otrzymanych danych.
    connect(serialHandler, &SerialPortHandler::newData,
            this, [this](double x_g, double y_g, double z_g) {
                float pitch = std::atan2(y_g, z_g);
                float roll  = std::atan2(x_g, z_g);
                ui->openGLWidget->setRotationAngles(pitch, roll);
            });
}

/**
 * @brief Destruktor klasy MainWindow.
 * Usuwa dynamicznie alokowane obiekty i zwalnia zasoby.
 */
MainWindow::~MainWindow()
{
    delete spinController;
    delete serialHandler;
    delete translator;
    delete simulationController;
    delete ui;
}

/**
 * @brief Slot obsługujący kliknięcie przycisku "Charts".
 * Tworzy i wyświetla okno wykresów, jeśli jeszcze nie istnieje lub zostało zamknięte.
 */
void MainWindow::on_pushButtonCharts_clicked()
{
    ChartWindow::openChartWindow(translator,serialHandler);
}

/**
 * @brief Slot obsługujący kliknięcie przycisku zmiany języka.
 * Wywołuje zmianę języka aplikacji poprzez Translator.
 */
void MainWindow::on_pushButtonLanguage_clicked()
{
    translator->toggleLanguage();
    ui->retranslateUi(this);
}

/**
 * @brief Slot obsługujący kliknięcie przycisku "Start".
 * Włącza/wyłącza odczyt danych z portu szeregowego i aktualizuje wygląd przycisku.
 */
void MainWindow::on_pushButtonStart_clicked()
{
    if (!serialHandler->isConnected())
        return;

    serialHandler->toggleReading();

    if (serialHandler->isReading()) {
        ui->pushButtonStart->setText(tr("Stop"));
        ui->pushButtonStart->setStyleSheet("background-color: red; color: white;");
    } else {
        ui->pushButtonStart->setText(tr("Start"));
        ui->pushButtonStart->setStyleSheet("background-color: green; color: white;");
    }
}

/**
 * @brief Slot obsługujący kliknięcie przycisku "Search".
 * Odświeża listę dostępnych portów szeregowych w comboboxie.
 */
void MainWindow::on_pushButtonSearch_clicked()
{
    ui->comboBoxDevices->clear();
    ui->comboBoxDevices->addItems(serialHandler->availablePorts());
}

/**
 * @brief Slot obsługujący kliknięcie przycisku "Connect".
 * Otwiera lub zamyka połączenie z wybranym portem szeregowym.
 */
void MainWindow::on_pushButtonConnect_clicked()
{
    serialHandler->updateConnectionUI(ui->comboBoxDevices->currentText(),
                                      ui->pushButtonConnect,
                                      ui->pushButtonStart);
}

