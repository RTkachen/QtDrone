#include "chartwindow.hpp"
#include "ui_chartwindow.h"

ChartWindow::ChartWindow(QWidget *parent)
    : QWidget(parent), ui(new Ui::ChartWindow)
{
    ui->setupUi(this);
    QVBoxLayout *layout = new QVBoxLayout(this);
    QString labels[3] = {"X to t", "Y to t", "Z to t"};

    for (int i = 0; i < 3; ++i) {
        series[i] = new QLineSeries();
        chartViews[i] = new QChartView(new QChart(), this);
        setupChart(i, labels[i]);
        layout->addWidget(chartViews[i]);
    }
}

ChartWindow::~ChartWindow() {
    delete ui;
}

void ChartWindow::setupChart(int index, const QString &title)
{
    QChart *chart = chartViews[index]->chart();
    chart->legend()->hide();
    chart->setTitle(title);
    chart->addSeries(series[index]);

    axisX[index] = new QValueAxis();
    axisX[index]->setRange(0, 15.0);
    axisX[index]->setLabelFormat("%.1f");
    axisX[index]->setTitleText("Time (s)");

    axisY[index] = new QValueAxis();
    axisY[index]->setRange(-2.0, 2.0);
    axisY[index]->setTitleText("Value");

    chart->addAxis(axisX[index], Qt::AlignBottom);
    chart->addAxis(axisY[index], Qt::AlignLeft);
    series[index]->attachAxis(axisX[index]);
    series[index]->attachAxis(axisY[index]);

    chartViews[index]->setRenderHint(QPainter::Antialiasing);
}

float ChartWindow::getCurrentTime() const {
    return sampleCount * 0.05f;
}

void ChartWindow::addData(float x, float y, float z)
{
    const float time = getCurrentTime();

    series[0]->append(time, x);
    series[1]->append(time, y);
    series[2]->append(time, z);
    ++sampleCount;

    for (int i = 0; i < 3; ++i) {
        if (series[i]->count() > maxSamples)
            series[i]->remove(0);

        // Przesuwaj zakres osi X zgodnie z czasem
        axisX[i]->setRange(time - 15.0f, time);
    }
}

void ChartWindow::updateData(double x, double y, double z) {
    addData(static_cast<float>(x), static_cast<float>(y), static_cast<float>(z));
}
