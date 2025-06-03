#ifndef TRANSLATOR_H
#define TRANSLATOR_H
#include <QPushButton>

#include <QObject>
#include <QPushButton>
#include <QTranslator>

/**
 * @brief Klasa Translator odpowiedzialna za dynamiczne przełączanie języka GUI.
 *
 * Klasa ta zmienia tłumaczenie całej aplikacji poprzez ładowanie odpowiednich
 * plików .qm (tłumaczeń) z zasobów. Jeśli aktualnie widoczna jest flaga polska
 * (przycisk pokazuje tekst "Polish"), kliknięcie powoduje załadowanie tłumaczenia angielskiego,
 * a jeśli flaga brytyjska (tekst "English"), to przełączenie na tłumaczenie polskie.
 */
class Translator : public QObject {
    Q_OBJECT

public:
    /**
     * @brief Konstruktor klasy Translator.
     *
     * Inicjalizuje przycisk zmiany języka oraz ustawia domyślny stan – aplikacja działa po angielsku,
     * dlatego początkowo przycisk pokazuje polską flagę, sugerując że kliknięcie przełączy na angielski.
     *
     * @param button Wskaźnik do przycisku, poprzez który zmieniany będzie język.
     * @param parent Obiekt nadrzędny.
     */
    explicit Translator(QPushButton* button, QObject* parent = nullptr);

    /**
     * @brief Przełącza język interfejsu.
     *
     * Jeśli aktualnie przycisk wskazuje "Polish" (czyli wyświetla polską flagę),
     * zostanie załadowane tłumaczenie angielskie, a przycisk zmieni ikonę na flagę brytyjską
     * i tekst na "English". W przeciwnym razie – zostanie załadowane tłumaczenie polskie,
     * ustawiana jest flaga polska i tekst "Polish".
     */
    void toggleLanguage();

signals:
    /**
     * @brief Sygnał emitowany po zmianie języka.
     *
     * Po poprawnym przełączeniu tłumaczenia zostanie wyemitowany sygnał, który powinien
     * obsłużyć wszystkie otwarte okna (np. MainWindow, ChartWindow) i wywołać u nich ich
     * metodę aktualizującą teksty w interfejsie.
     */
    void languageChanged();

private:
    bool m_isPolishActive;           ///< Flaga informująca o tym czy program jest w języku polskim.
    QPushButton* m_button;           ///< Przycisk odpowiedzialny za zmianę języka.
    QTranslator* m_currentTranslator;///< Aktualnie zainstalowany tłumacz.
};

#endif // TRANSLATOR_H
