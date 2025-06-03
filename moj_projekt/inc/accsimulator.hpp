#ifndef ACCSIMULATOR_HPP
#define ACCSIMULATOR_HPP

#include <QObject>
#include <QTimer>

/**
 * @brief Klasa generująca dane symulacyjne XYZ.
 *
 * Generuje pseudolosowe dane przy pomocy timera z ustalonym interwałem czasowym.
 * Symuluje dane akcelerometru w zakresie zbliżonym do [-2.00, 2.00].
 */
class AccSimulator : public QObject
{
    Q_OBJECT

public:
    /**
     * @brief Konstruktor klasy AccSimulator.
     * @param parent Wskaźnik do obiektu nadrzędnego.
     */
    explicit AccSimulator(QObject *parent = nullptr);

    /**
     * @brief Rozpoczyna generowanie danych.
     */
    void start();

    /**
     * @brief Zatrzymuje generowanie danych.
     */
    void stop();

    /**
 * @brief Sprawdza, czy generowanie danych jest aktywne.
 *
 * @retval true - jeśli generowanie danych jest aktywne.
 * @retval false - jeśli generowanie danych nie jest aktywne.
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

private slots:
    /**
     * @brief Główna funkcja generująca dane wywoływana przez QTimer.
     */
    void generateData();

private:
    QTimer *timer;     ///< Timer do okresowego wywoływania generacji danych.
    bool running = false; ///< Flaga wskazująca czy generowanie jest aktywne.

    /**
     * @brief Generuje losową liczbę zmiennoprzecinkową z danego zakresu.
     * @param min Wartość minimalna.
     * @param max Wartość maksymalna.
     * @return Losowa wartość z przedziału [min, max].
     */
    double randomInRange(double min, double max);
};

#endif // ACCSIMULATOR_HPP
