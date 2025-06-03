#include "translator.hpp"
#include "translator.hpp"
#include <QIcon>
#include <QApplication>
#include <QDebug>

Translator::Translator(QPushButton* button, QObject* parent)
    : QObject(parent)
    , m_button(button)
    , m_currentTranslator(nullptr)
    , m_isPolishActive(false)
{
    // Przy uruchomieniu aplikacji działamy w wersji angielskiej,
    // dlatego przycisk pokazuje polską flagę – kliknięcie spowoduje przełączenie na angielskie tłumaczenie.
    m_button->setIcon(QIcon(":/images/poland_flag.png"));
    m_button->setText("Polish");
}
/**
 * @brief Przełącza język interfejsu aplikacji.
 *
 * Funkcja usuwa aktualnie zainstalowany translator (jeśli istnieje) i tworzy nowego translatora.
 * Następnie, w zależności od stanu zmiennej <code>m_isPolishActive</code>:
 * - Jeśli interfejs działa po angielsku (<code>m_isPolishActive == false</code>), ładuje
 *   polski plik tłumaczeń (":/languages/moj_projekt_pl_150.qm") i instaluje translator. W przypadku
 *   powodzenia ustawia flagę na <code>true</code>, zmienia ikonę przycisku na UK oraz ustawia tekst
 *   przycisku na "English" – wskazując, że kliknięcie przełączy interfejs na angielski.
 * - Jeśli interfejs jest już po polsku (<code>m_isPolishActive == true</code>), usuwa translator,
 *   aby przywrócić domyślne, angielskie ciągi oraz ustawia ikonę przycisku na flagę Polski i tekst na "Polish".
 *
 * Po zmianie języka funkcja emituje sygnał <code>languageChanged()</code> w celu powiadomienia
 * pozostałych komponentów aplikacji o zmianie języka.
 */
void Translator::toggleLanguage() {
    // Usuń poprzedni translator, jeśli istnieje.
    if (m_currentTranslator) {
        qApp->removeTranslator(m_currentTranslator);
        delete m_currentTranslator;
        m_currentTranslator = nullptr;
    }

    // Utwórz nowego translatora.
    m_currentTranslator = new QTranslator();
    bool loaded = false;

    if (!m_isPolishActive) {
        // Aktualnie interfejs działa po angielsku – przełączamy na polski.
        loaded = m_currentTranslator->load(":/languages/moj_projekt_pl_150.qm");
        if (loaded) {
            qApp->installTranslator(m_currentTranslator);
            m_isPolishActive = true;
            // Teraz interfejs jest po polsku. Przyciskiem dajemy możliwość przełączenia na angielski.
            m_button->setIcon(QIcon(":/images/uk_flag.png"));
            m_button->setText("English");
        } else {
            qDebug() << "Nie udało się załadować tłumaczenia polskiego.";
            delete m_currentTranslator;
            m_currentTranslator = nullptr;
            return;
        }
    } else {
        // Aktualnie interfejs jest po polsku – przełączamy z powrotem na wersję angielską.
        // Dla języka angielskiego nie trzeba instalować translatora – działają domyślne ciągi.
        m_isPolishActive = false;
        m_button->setIcon(QIcon(":/images/poland_flag.png"));
        m_button->setText("Polish");
    }

    // Powiadomienie, że język został zmieniony.
    emit languageChanged();
}
