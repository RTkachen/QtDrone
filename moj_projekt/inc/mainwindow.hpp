/**
 * @file mainwindow.hpp
 * @brief Główne okno aplikacji odpowiedzialne za interakcję z użytkownikiem.
 *
 * Klasa MainWindow tworzy i zarządza interfejsem graficznym aplikacji Qt, która
 * wyświetla obiekt 3D sterowany danymi z akcelerometru. Zawiera kontrolki GUI
 * umożliwiające użytkownikowi: symulację danych, zmianę języka, wyszukiwanie portów
 * szeregowych, połączenie z portem szeregowym oraz rozpoczęcie odbierania danych.
 */

#include "simulationcontroller.hpp"
#include "SerialPortHandler.hpp"
#include "SpinBoxController.hpp"
#include "chartwindow.hpp"
#include "translator.hpp"
#include <QMainWindow>
#include <QSerialPort>
#include "D3widget.hpp"
#include <cmath>

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

/**
 * @class MainWindow
 * @brief Klasa reprezentująca główne okno aplikacji.
 */
class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    /**
     * @brief Konstruktor MainWindow.
     * @param parent Wskaźnik do rodzica (domyślnie nullptr).
     */
    MainWindow(QWidget *parent = nullptr);

    /**
     * @brief Destruktor klasy MainWindow.
     */
    ~MainWindow();

private slots:
    /**
     * @brief Slot obsługujący kliknięcie przycisku otwierającego okno wykresów.
     */
    void on_pushButtonCharts_clicked();

    /**
     * @brief Slot obsługujący kliknięcie przycisku zmieniającego język interfejsu.
     */
    void on_pushButtonLanguage_clicked();

    /**
     * @brief Slot obsługujący kliknięcie przycisku rozpoczynającego odbieranie danych.
     */
    void on_pushButtonStart_clicked();

    /**
     * @brief Slot obsługujący kliknięcie przycisku wyszukiwania dostępnych portów szeregowych.
     */
    void on_pushButtonSearch_clicked();

    /**
     * @brief Slot obsługujący kliknięcie przycisku łączenia z wybranym portem szeregowym.
     */
    void on_pushButtonConnect_clicked();

private:
    SpinBoxController* spinController;         ///< Kontroler dla wyświetlaczy wartości przyspieszenia (x, y, z).
    SerialPortHandler *serialHandler;            ///< Handler odpowiedzialny za komunikację przez port szeregowy.
    ChartWindow* chartWindow = nullptr;          ///< Wskaźnik na okno wykresów (otwierane dynamicznie).
    Translator* translator = nullptr;            ///< Tłumacz interfejsu użytkownika (zmiana języka).
    Ui::MainWindow *ui;                          ///< Wskaźnik do interfejsu użytkownika wygenerowanego przez Qt Designer.
    SimulationController* simulationController = nullptr; ///< Kontroler symulacji danych akcelerometru.
};
