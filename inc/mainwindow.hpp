#ifndef MAINWINDOW_HPP
#define MAINWINDOW_HPP
#include "simulationcontroller.hpp"
#include "SpinBoxController.hpp"
#include "chartwindow.hpp"
#include "translate.hpp"
#include <QMainWindow>
#include <QSerialPort>
QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_pushButtonCharts_clicked();
    void on_pushButtonLanguage_clicked();
    void on_pushButtonStart_clicked();
private:
    SpinBoxController* spinController;
    ChartWindow* chartWindow = nullptr;
    Translator* translator = nullptr;
    Ui::MainWindow *ui;
    SimulationController* simulationController;
};
#endif // MAINWINDOW_HPP
