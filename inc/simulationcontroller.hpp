#ifndef SIMULATIONCONTROLLER_HPP
#define SIMULATIONCONTROLLER_HPP

#include <QObject>

class AccSimulator;
class SimulationController : public QObject
{
    Q_OBJECT

public:
    explicit SimulationController(QObject *parent = nullptr);

    void startSimulation();
    void stopSimulation();
    bool isRunning() const;
signals:
    void newData(double x, double y, double z);
private:
    AccSimulator* simulator; ///< Wskaźnik do obiektu generującego dane symulacyjne.
};

#endif // SIMULATIONCONTROLLER_HPP
