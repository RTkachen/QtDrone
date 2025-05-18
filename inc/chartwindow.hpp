#ifndef CHARTWINDOW_HPP
#define CHARTWINDOW_HPP

#include <QWidget>
#include <QtCharts>
#include <QChartView>

QT_BEGIN_NAMESPACE
namespace Ui { class ChartWindow; }
QT_END_NAMESPACE
class ChartWindow : public QWidget {
    Q_OBJECT

public:
    explicit ChartWindow(QWidget *parent = nullptr);

    ~ChartWindow();

public slots:
    void updateData(double x, double y, double z);

private:
    void setupChart(int index, const QString &title);
    void addData(float x, float y, float z);

    Ui::ChartWindow *ui;
    QChartView* chartViews[3];
    QLineSeries* series[3];
    QValueAxis* axisX[3];
    QValueAxis* axisY[3];
    int sampleCount = 0;
    const int maxSamples = 300; // 15s / 0.05s
    float getCurrentTime() const;
};
#endif // CHARTWINDOW_HPP
