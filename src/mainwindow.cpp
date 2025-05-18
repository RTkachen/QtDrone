#include "mainwindow.hpp"
#include "ui/ui_mainwindow.h"
#include <QDebug>
#include <QSerialPortInfo>
#include <QList>
#include <QComboBox>
#include <QDateTime>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)  // Inicjalizacja interfejsu użytkownika
{
    ui->setupUi(this);  // Konfiguracja interfejsu
    translator = new Translator(ui->pushButtonLanguage);  // Inicjalizacja tłumacza dla przycisku zmiany języka
    spinController = new SpinBoxController(ui->NumberX,
                                           ui->NumberY,
                                           ui->NumberZ,
                                           0.01, -1.23, 3.14);

    simulationController = new SimulationController(this);
    connect(simulationController, &SimulationController::newData,
            spinController,      &SpinBoxController::updateAll);

    //this->device = new QSerialPort(this);  // (Opcjonalnie) Inicjalizacja portu szeregowego (komentarz, bo nieaktywne)
}

MainWindow::~MainWindow()
{
    // Usuwanie obiektów, aby zwolnić pamięć
    delete translator;
    delete simulationController;
    delete ui;
}

// Funkcja obsługująca kliknięcie przycisku "Charts" - otwiera nowe okno z wykresami
void MainWindow::on_pushButtonCharts_clicked()
{
    // Jeśli okno wykresów nie zostało jeszcze utworzone lub zostało zamknięte
    if (!chartWindow || !chartWindow->isVisible()) {
        chartWindow = new ChartWindow();  // Tworzenie nowego okna z wykresami
        chartWindow->setAttribute(Qt::WA_DeleteOnClose);  // Ustawienie, aby okno było usuwane po zamknięciu

        // Po zamknięciu okna wykresów ustaw wskaźnik na nullptr
        connect(chartWindow, &QObject::destroyed, this, [this]() {
            chartWindow = nullptr;
        });

        // Podłączenie sygnału z symulacji do metody aktualizującej dane w wykresach
        connect(simulationController, &SimulationController::newData,
                chartWindow, &ChartWindow::updateData);

        chartWindow->show();  // Wyświetlanie okna z wykresami
    }
}

// Funkcja obsługująca kliknięcie przycisku zmiany języka
void MainWindow::on_pushButtonLanguage_clicked()
{
    translator->toggleLanguage();  // Zmiana języka (toggle)
}

// Funkcja obsługująca kliknięcie przycisku "Start" (rozpoczęcie lub zatrzymanie symulacji)
void MainWindow::on_pushButtonStart_clicked()
{
    if (simulationController->isRunning()) {
        simulationController->stopSimulation();  // Zatrzymanie symulacji
        ui->pushButtonStart->setText("Start");  // Zmiana tekstu na "Start"
        ui->pushButtonStart->setStyleSheet("");  // Resetowanie stylu przycisku
    } else {
        simulationController->startSimulation();  // Rozpoczęcie symulacji
        ui->pushButtonStart->setText("Close");  // Zmiana tekstu na "Close"
        ui->pushButtonStart->setStyleSheet("background-color: red; color: white;");  // Ustawienie czerwonego tła przycisku
    }
}

