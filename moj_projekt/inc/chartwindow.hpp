#ifndef CHARTWINDOW_HPP
#define CHARTWINDOW_HPP

#include <QWidget>
#include <QtCharts>
#include <QChartView>
#include "translator.hpp"

QT_BEGIN_NAMESPACE
namespace Ui { class ChartWindow; }
QT_END_NAMESPACE

static const char* ChartLabelX     = QT_TRANSLATE_NOOP("ChartWindow", "X vs Time");
static const char* ChartLabelY     = QT_TRANSLATE_NOOP("ChartWindow", "Y vs Time");
static const char* ChartLabelZ     = QT_TRANSLATE_NOOP("ChartWindow", "Z vs Time");
static const char* AxisTimeLabel = QT_TRANSLATE_NOOP("ChartWindow", "Time (s)");
static const char* AxisValueLabel = QT_TRANSLATE_NOOP("ChartWindow", "Acceleration (g)");

/**
 * @brief Klasa ChartWindow odpowiedzialna za wyświetlanie wykresów dla sygnałów X, Y oraz Z.
 *
 * Klasa ta tworzy trzy wykresy opartych na QChartView oraz QLineSeries, a także
 * posiada metody do aktualizacji danych, konfiguracji wykresów. Dodatkowo udostępnia
 * statyczną metodę openChartWindow() do samodzielnego wyświetlenia okna wykresów.
 */
class ChartWindow : public QWidget {
    Q_OBJECT

public:

    /**
     * @brief Konstruktor ChartWindow.
     * Tworzy interfejs użytkownika i ustawia trzy wykresy, przy czym każdy wykres dla osi X, Y i Z.
     * Ustawienia wykresów, takie jak zakres osi i tytuły, są tłumaczalne (wykorzystuje QObject::tr()).
     * @param parent Wskaźnik do obiektu nadrzędnego (domyślnie nullptr).
     */
    explicit ChartWindow(Translator* translator, QWidget *parent = nullptr);

    /**
     * @brief Destruktor ChartWindow.
     */
    ~ChartWindow();

    /**
     * @brief Statyczna metoda otwierająca okno wykresów.
     *
     * Metoda ta tworzy nową instancję ChartWindow (jeśli nie istnieje lub nie jest widoczna),
     * ustawia odpowiednio atrybuty oraz łączy sygnał newData obiektu SerialPortHandler z metodą updateData.
     *
     * @param serialHandler Wskaźnik do obiektu SerialPortHandler, którego sygnały będą przesyłane do wykresów.
     */
    static void openChartWindow(Translator* translator,QObject *serialHandler);

public slots:
    /**
     * @brief Aktualizuje dane wykresów na podstawie otrzymanych wartości.
     *
     * Metoda ta dodaje kolejne punkty do serii danych na wykresach.
     *
     * @param x Wartość dla osi X.
     * @param y Wartość dla osi Y.
     * @param z Wartość dla osi Z.
     */
    void updateData(double x, double y, double z);

    /**
     * @brief Aktualizuje tłumaczenia interfejsu okna wykresów.
     *
     * Slot wywoływany po zmianie języka, który odświeża teksty tytułów wykresów oraz etykiety osi.
     */
    void updateTranslations();

private:
    /**
     * @brief Inicjalizuje wykres dla danej serii.
     *
     * Ustawia tytuł (przy użyciu QObject::tr() dla tłumaczeń), zakres osi X (0 do 30
     * sekund) oraz domyślne etykiety osi, które są również tłumaczalne.
     *
     * @param index Indeks wykresu (0 = X, 1 = Y, 2 = Z).
     * @param title Tytuł wykresu.
     */
    void setupChart(int index, const QString &title);

    /**
     * @brief Dodaje dane do wykresów.
     *
     * Dodaje nowe punkty do każdej serii i dba o przesuwanie zakresu osi X, by wyświetlał
     * okres 30 sekund (600 próbek) oraz usuwa starsze dane, gdy liczba próbek przekroczy ten limit.
     *
     * @param x Wartość X.
     * @param y Wartość Y.
     * @param z Wartość Z.
     */
    void addData(float x, float y, float z);

    /**
     * @brief Oblicza aktualny czas w sekundach na podstawie liczby próbek.
     *
     * @return Czas w sekundach.
     */
    float getCurrentTime() const;

    Translator* m_translator;

    Ui::ChartWindow *ui;
    QChartView* chartViews[3];
    QLineSeries* series[3];
    QValueAxis* axisX[3];
    QValueAxis* axisY[3];
    int sampleCount = 0;
    const int maxSamples = 600; ///< 30s / 0.05s

};
#endif // CHARTWINDOW_HPP
