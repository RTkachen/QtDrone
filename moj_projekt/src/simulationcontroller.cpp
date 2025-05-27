#include "simulationcontroller.hpp"
#include "accsimulator.hpp"

SimulationController::SimulationController(QObject *parent)
    : QObject(parent)
{
    // Tworzenie instancji symulatora akcelerometru
    simulator = new AccSimulator(this);

    // Połączenie sygnału newData z symulatora z metodą emitującą ten sygnał z klasy SimulationController
    connect(simulator, &AccSimulator::newData, this, [this](double x, double y, double z) {
        emit newData(x, y, z);  // Emitowanie nowego sygnału z danymi (x, y, z)
    });
}

// Funkcja rozpoczynająca symulację
void SimulationController::startSimulation()
{
    simulator->start();  // Uruchomienie symulatora
}

// Funkcja zatrzymująca symulację
void SimulationController::stopSimulation()
{
    simulator->stop();  // Zatrzymanie symulatora
}

// Funkcja sprawdzająca, czy symulacja jest aktywna
bool SimulationController::isRunning() const
{
    return simulator->isRunning();  // Zwrócenie stanu symulacji (czy jest uruchomiona)
}
