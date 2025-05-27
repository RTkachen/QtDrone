/**
 * @file SpinBoxController.hpp
 * @brief Kontroler do aktualizacji wartości przyspieszeń w polach QDoubleSpinBox.
 *
 * Klasa SpinBoxController służy do prezentacji aktualnych danych z akcelerometru
 * (dla osi X, Y, Z) w postaci liczbowej w trzech polach QDoubleSpinBox.
 * Może być również wykorzystywana w trybie symulacji.
 */

#ifndef SPINBOXCONTROLLER_HPP
#define SPINBOXCONTROLLER_HPP
#include <QObject>
#include <QDoubleSpinBox>

/**
 * @class SpinBoxController
 * @brief Klasa zarządzająca trzema QDoubleSpinBox pokazującymi wartości akcelerometru.
 */
class SpinBoxController : public QObject
{
    Q_OBJECT

public:
    /**
     * @brief Konstruktor kontrolera.
     * @param boxX Wskaźnik na spinbox wyświetlający oś OX.
     * @param boxY Wskaźnik na spinbox wyświetlający oś OY.
     * @param boxZ Wskaźnik na spinbox wyświetlający oś OZ.
     * @param initialX Początkowa wartość osi OX (domyślnie 0.0).
     * @param initialY Początkowa wartość osi OY (domyślnie 0.0).
     * @param initialZ Początkowa wartość osi OZ (domyślnie 0.0).
     */
    explicit SpinBoxController(QDoubleSpinBox* boxX,
                               QDoubleSpinBox* boxY,
                               QDoubleSpinBox* boxZ,
                               double initialX = 0.0,
                               double initialY = 0.0,
                               double initialZ = 0.0);

public slots:
    /**
     * @brief Aktualizuje wartości we wszystkich trzech spinboxach.
     * @param x Nowa wartość dla osi OX.
     * @param y Nowa wartość dla osi OY.
     * @param z Nowa wartość dla osi OZ.
     */
    void updateAll(double x, double y, double z);

private:
    /**
     * @brief Pomocnicza funkcja do ustawienia wartości w jednym spinboxie.
     * @param box Wskaźnik do QDoubleSpinBox.
     * @param value Wartość do ustawienia.
     */
    void updateDisplay(QDoubleSpinBox* box, double value);

    QDoubleSpinBox* m_boxX; ///< Spinbox dla osi OX
    QDoubleSpinBox* m_boxY; ///< Spinbox dla osi OY
    QDoubleSpinBox* m_boxZ; ///< Spinbox dla osi OZ
};

#endif // SPINBOXCONTROLLER_HPP
