#include "SpinBoxController.hpp"

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
    // Wyświetl od razu wartości początkowe
    updateAll(initialX, initialY, initialZ);
}

void SpinBoxController::updateAll(double x, double y, double z)
{
    updateDisplay(m_boxX, x);
    updateDisplay(m_boxY, y);
    updateDisplay(m_boxZ, z);
}

void SpinBoxController::updateDisplay(QDoubleSpinBox* box, double value)
{
    // Po prostu ustaw wartość – prefix/suffix/decimals/range
    // są już skonfigurowane w Designerze.
    box->setValue(value);
}
