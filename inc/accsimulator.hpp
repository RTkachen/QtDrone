#ifndef ACCSIMULATOR_HPP
#define ACCSIMULATOR_HPP

#include <QObject>
#include <QTimer>

class AccSimulator : public QObject
{
    Q_OBJECT

public:

    explicit AccSimulator(QObject *parent = nullptr);
    void start();
    void stop();
    bool isRunning() const;

signals:
    void newData(double x, double y, double z);

private slots:
    void generateData();

private:
    QTimer *timer;
    bool running = false;
    double randomInRange(double min, double max);
};

#endif // ACCSIMULATOR_HPP
