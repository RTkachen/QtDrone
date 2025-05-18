#include "accsimulator.hpp"
#include <QRandomGenerator>
AccSimulator::AccSimulator(QObject *parent)
    : QObject(parent)
{
    // Tworzenie obiektu QTimer, który będzie odpowiedzialny za generowanie danych
    timer = new QTimer(this);

    // Po każdym upływie czasu (timeout) wywołana zostanie funkcja generateData
    connect(timer, &QTimer::timeout, this, &AccSimulator::generateData);
}

// Funkcja uruchamiająca symulator
void AccSimulator::start()
{
    if (!running) {
        // Rozpoczęcie generowania danych co 50ms (20Hz)
        timer->start(50);
        running = true; // Ustawienie flagi, że symulator działa
    }
}

// Funkcja zatrzymująca symulator
void AccSimulator::stop()
{
    if (running) {
        // Zatrzymanie timera, który generuje dane
        timer->stop();
        running = false; // Ustawienie flagi, że symulator nie działa
    }
}

// Funkcja sprawdzająca, czy symulator jest uruchomiony
bool AccSimulator::isRunning() const
{
    return running; // Zwrócenie stanu symulatora (czy jest uruchomiony)
}

// Funkcja generująca losową wartość z zakresu [min, max]
double AccSimulator::randomInRange(double min, double max) {
    return min + QRandomGenerator::global()->generateDouble() * (max - min);
}

// Funkcja wywoływana co 50ms (timer timeout), generująca dane dla trzech osi (x, y, z)
void AccSimulator::generateData()
{
    // Generowanie losowych danych dla każdej z osi w zakresie [-2.0, 2.0]
    double x = randomInRange(-2.0, 2.0);
    double y = randomInRange(-2.0, 2.0);
    double z = randomInRange(-2.0, 2.0);

    // Emitowanie nowo wygenerowanych danych (sygnał do innych komponentów)
    emit newData(x, y, z);
}
