/**
    * @file SpinBoxController.cpp
         * @brief Implementacja klasy SpinBoxController odpowiedzialnej za aktualizację wartości QDoubleSpinBox na podstawie danych z akcelerometru.
             */

#include "SpinBoxController.hpp"

    /**
 * @brief Konstruktor klasy SpinBoxController.
 * Inicjalizuje wskaźniki na pola typu QDoubleSpinBox i ustawia wartości początkowe.
 *
 * @param boxX Wskaźnik na pole QDoubleSpinBox dla osi X.
 * @param boxY Wskaźnik na pole QDoubleSpinBox dla osi Y.
 * @param boxZ Wskaźnik na pole QDoubleSpinBox dla osi Z.
 * @param initialX Wartość początkowa dla osi X.
 * @param initialY Wartość początkowa dla osi Y.
 * @param initialZ Wartość początkowa dla osi Z.
 */
    SpinBoxController::SpinBoxController(QDoubleSpinBox* boxX,
                                         QDoubleSpinBox* boxY,
                                         QDoubleSpinBox* boxZ,
                                         double initialX,
                                         double initialY,
                                         double initialZ)
    : m_boxX(boxX)
, m_boxY(boxY)
, m_boxZ(boxZ)
{
    updateAll(initialX, initialY, initialZ);
}

/**
 * @brief Aktualizuje wszystkie trzy pola (X, Y, Z) wartościami podanymi jako argumenty.
 *
 * @param x Nowa wartość osi X.
 * @param y Nowa wartość osi Y.
 * @param z Nowa wartość osi Z.
 */
void SpinBoxController::updateAll(double x, double y, double z)
{
    updateDisplay(m_boxX, x);
    updateDisplay(m_boxY, y);
    updateDisplay(m_boxZ, z);
}

/**
 * @brief Ustawia wartość w podanym polu QDoubleSpinBox.
 *
 * @param box Wskaźnik na pole spinbox, które ma zostać zaktualizowane.
 * @param value Wartość do ustawienia w polu.
 */
void SpinBoxController::updateDisplay(QDoubleSpinBox* box, double value)
{
    box->setValue(value);
}
