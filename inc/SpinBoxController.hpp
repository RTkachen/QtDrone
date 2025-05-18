#ifndef SPINBOXCONTROLLER_HPP
#define SPINBOXCONTROLLER_HPP

#include <QObject>
#include <QDoubleSpinBox>

class SpinBoxController : public QObject
{
    Q_OBJECT

public:
    explicit SpinBoxController(QDoubleSpinBox* boxX,
                               QDoubleSpinBox* boxY,
                               QDoubleSpinBox* boxZ,
                               double initialX = 0.0,
                               double initialY = 0.0,
                               double initialZ = 0.0);

public slots:
    void updateAll(double x, double y, double z);

private:
    void updateDisplay(QDoubleSpinBox* box, double value);

    QDoubleSpinBox* m_boxX;
    QDoubleSpinBox* m_boxY;
    QDoubleSpinBox* m_boxZ;
};

#endif // SPINBOXCONTROLLER_HPP
