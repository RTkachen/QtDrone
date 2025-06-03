#include "chartwindow.hpp"
#include "ui_chartwindow.h"
#include <QtCharts>

/**
 * @brief Konstruktor klasy ChartWindow.
 *
 * Tworzy interfejs użytkownika, ustawia layout, inicjalizuje trzy wykresy (dla osi X, Y i Z)
 * o tytułach (tłumaczalnych), zakresach osi oraz stylem renderowania (antyaliasing).
 *
 * Domyślnie wykresy pokazują okres 30 sekund przy 600 próbkach (próbkowanie co 0.05 sekundy).
 *
 * @param parent Wskaźnik do obiektu nadrzędnego (domyślnie nullptr).
 */
ChartWindow::ChartWindow(Translator* translator, QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::ChartWindow)
    , m_translator(translator)
{
    ui->setupUi(this);
    QVBoxLayout *layout = new QVBoxLayout(this);

    // Używamy tłumaczalnych tytułów wykresów.
    QStringList labels;
    labels << tr("X vs Time") << tr("Y vs Time") << tr("Z vs Time");

    for (int i = 0; i < 3; ++i) {
        series[i] = new QLineSeries();
        chartViews[i] = new QChartView(new QChart(), this);
        setupChart(i, labels.at(i));
        layout->addWidget(chartViews[i]);
    }
    updateTranslations();
    if(m_translator) {
        connect(m_translator, &Translator::languageChanged,
                this, &ChartWindow::updateTranslations);
    }
}

/**
 * @brief Destruktor klasy ChartWindow.
 *
 * Usuwa utworzony interfejs, zwalniając pamięć.
 */
ChartWindow::~ChartWindow()
{
    delete ui;
}

/**
 * @brief Statyczna metoda otwierająca okno wykresów.
 *
 * Jeśli okno nie istnieje lub nie jest widoczne, tworzy nową instancję ChartWindow,
 * ustawia atrybut WA_DeleteOnClose, łączy sygnał z SerialPortHandler z metodą updateData,
 * a następnie wyświetla okno.
 *
 * @param serialHandler Wskaźnik do obiektu odpowiedzialnego za obsługę portu szeregowego.
 */
void ChartWindow::openChartWindow(Translator* translator, QObject *serialHandler)
{
    static ChartWindow *instance = nullptr;
    if (!instance || !instance->isVisible()) {
        instance = new ChartWindow(translator);
        instance->setAttribute(Qt::WA_DeleteOnClose);

        QObject::connect(instance, &QObject::destroyed, [=]() {
            instance = nullptr;
        });

        // Zakładamy, że serialHandler emituje sygnał newData(double, double, double)
        QObject::connect(serialHandler, SIGNAL(newData(double,double,double)),
                         instance, SLOT(updateData(double,double,double)));

        instance->show();
    } else {
        instance->raise();
        instance->activateWindow();
    }
}

/**
 * @brief Inicjalizuje wykres dla danego indeksu i ustawia jego konfigurację.
 *
 * Ustawia tytuł wykresu (przy użyciu metody tr(), aby był tłumaczalny), zakres osi X na 0–30 sekund,
 * etykiety osi (Time (s) oraz Value) oraz łączy serię danych z odpowiednimi osiami.
 *
 * @param index Indeks wykresu (0 = X, 1 = Y, 2 = Z).
 * @param title Tytuł wykresu, który jest tłumaczalny.
 */
void ChartWindow::setupChart(int index, const QString &title)
{
    QChart *chart = chartViews[index]->chart();
    chart->legend()->hide();
   // chart->setTitle(tr("%1").arg(title));
    chart->addSeries(series[index]);

    // Zakres osi X: 0 - 30 sekund
    axisX[index] = new QValueAxis();
    axisX[index]->setRange(0, 30.0);
    axisX[index]->setLabelFormat("%.1f");
    axisX[index]->setTitleText(tr("Time (s)"));

    axisY[index] = new QValueAxis();
    axisY[index]->setRange(-2.0, 2.0);
    axisY[index]->setTitleText(tr("Acceleration (g)"));

    chart->addAxis(axisX[index], Qt::AlignBottom);
    chart->addAxis(axisY[index], Qt::AlignLeft);
    series[index]->attachAxis(axisX[index]);
    series[index]->attachAxis(axisY[index]);

    chartViews[index]->setRenderHint(QPainter::Antialiasing);
}

/**
 * @brief Oblicza aktualny czas na podstawie liczby próbek.
 *
 * Dla próbkowania co 0,05 sekundy, zwraca aktualny czas jako sampleCount * 0.05.
 *
 * @return Aktualny czas w sekundach.
 */
float ChartWindow::getCurrentTime() const
{
    return sampleCount * 0.05f;
}

/**
 * @brief Dodaje nowe dane do serii wykresów i aktualizuje zakres osi X.
 *
 * Do każdej serii (odpowiadającej osi X, Y i Z) dodaje nowy punkt o współrzędnych: czas, wartość.
 * Jeżeli liczba próbek przekroczy maxSamples (600 próbek, czyli 30 sekund), usuwa najstarsze punkty.
 * Zakres osi X jest ustawiany tak, by pokazywać ostatnie 30 sekund.
 *
 * @param x Wartość osi X.
 * @param y Wartość osi Y.
 * @param z Wartość osi Z.
 */
void ChartWindow::addData(float x, float y, float z)
{
    float time = getCurrentTime();
    series[0]->append(time, x);
    series[1]->append(time, y);
    series[2]->append(time, z);

    sampleCount++;
    if (sampleCount > maxSamples)
    {
        for (int i = 0; i < 3; ++i)
        {
            series[i]->removePoints(0, series[i]->count() - maxSamples);
        }
    }

    float startTime = std::max(0.0f, time - 30.0f);
    for (int i = 0; i < 3; ++i)
    {
        axisX[i]->setRange(startTime, time);
    }
}

/**
 * @brief Slot aktualizujący dane na wykresach.
 *
 * Rzutuje otrzymane wartości double na float i przekazuje je do metody addData,
 * która aktualizuje poszczególne serie wykresów.
 *
 * @param x Wartość osi X.
 * @param y Wartość osi Y.
 * @param z Wartość osi Z.
 */
void ChartWindow::updateData(double x, double y, double z)
{
    addData(static_cast<float>(x), static_cast<float>(y), static_cast<float>(z));
}
/**
 * @brief Aktualizuje tłumaczenia dynamicznych elementów wykresów w oknie.
 *
 * Funkcja ta aktualizuje elementy interfejsu zdefiniowane w pliku Designer poprzez
 * wywołanie <code>ui->retranslateUi(this)</code>. Następnie w pętli dla każdego z trzech
 * wykresów ustawia:
 *  - Tytuł wykresu na podstawie indeksu, korzystając z predefiniowanych stałych tłumaczeniowych:
 *    - 0: <code>ChartLabelX</code>
 *    - 1: <code>ChartLabelY</code>
 *    - 2: <code>ChartLabelZ</code>
 *  - Etykietę osi X, wykorzystując stałą <code>AxisTimeLabel</code>.
 *  - Etykietę osi Y, wykorzystując stałą <code>AxisValueLabel</code>.
 */
void ChartWindow::updateTranslations() {
    // Aktualizacja elementów zdefiniowanych w Designerze
    ui->retranslateUi(this);
    for (int i = 0; i < 3; ++i) {
        QChart *chart = chartViews[i]->chart();
        if (chart) {
            // Ustawienie tytułu wykresu w zależności od indeksu:
            switch (i) {
            case 0:
                chart->setTitle(tr(ChartLabelX));
                break;
            case 1:
                chart->setTitle(tr(ChartLabelY));
                break;
            case 2:
                chart->setTitle(tr(ChartLabelZ));
                break;
            }
        }
        // Uaktualnienie etykiety osi X:
        if (axisX[i]) {
            axisX[i]->setTitleText(tr(AxisTimeLabel));
        }
        // Uaktualnienie etykiety osi Y:
        if (axisY[i]) {
            axisY[i]->setTitleText(tr(AxisValueLabel));
        }
    }
}
