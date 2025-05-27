#ifndef SIMULATIONCONTROLLER_HPP
#define SIMULATIONCONTROLLER_HPP

#include <QObject>

class AccSimulator;

/**
 * @brief Kontroler symulacji akcelerometru.
 *
 * Odpowiada za uruchamianie i zatrzymywanie symulacji danych XYZ.
 * Emituje sygnał z danymi generowanymi przez AccSimulator.
 * Klasa została stworzona w celu testowania innych komponentów
 * (np. wykresów, pól edycyjnych) przed podłączeniem rzeczywistego urządzenia.
 */
class SimulationController : public QObject
{
    Q_OBJECT

public:
    /**
     * @brief Konstruktor klasy SimulationController.
     * @param parent Wskaźnik do obiektu nadrzędnego.
     */
    explicit SimulationController(QObject *parent = nullptr);

    /**
     * @brief Uruchamia symulację.
     */
    void startSimulation();

    /**
     * @brief Zatrzymuje symulację.
     */
    void stopSimulation();

    /**
 * @brief Sprawdza, czy symulacja jest aktywna.
 *
 * @retval true - jeśli symulacja trwa.
 * @retval false - jeśli symulacja nie jest aktywna.
 */
    bool isRunning() const;

signals:
    /**
     * @brief Sygnał z nowymi danymi XYZ.
     * @param x Wartość X.
     * @param y Wartość Y.
     * @param z Wartość Z.
     */
    void newData(double x, double y, double z);

private:
    AccSimulator* simulator; ///< Wskaźnik do obiektu generującego dane symulacyjne.
};

#endif // SIMULATIONCONTROLLER_HPP
