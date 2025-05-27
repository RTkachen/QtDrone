#include "translator.hpp"
#include "translator.hpp"
#include <QIcon>
#include <QApplication>
#include <QDebug>

Translator::Translator(QPushButton* button, QObject* parent)
    : QObject(parent)
    , m_button(button)
    , m_currentTranslator(nullptr)
{
    // Przy uruchomieniu aplikacji działamy w wersji angielskiej,
    // dlatego przycisk pokazuje polską flagę – kliknięcie spowoduje przełączenie na angielskie tłumaczenie.
    m_button->setIcon(QIcon(":/images/poland_flag.png"));
    m_button->setText("Polish");
}

void Translator::toggleLanguage() {
    // Jeśli istnieje już zainstalowany tłumacz, usuń go aby nie było konfliktów.
    if (m_currentTranslator) {
        qApp->removeTranslator(m_currentTranslator);
        delete m_currentTranslator;
        m_currentTranslator = nullptr;
    }

    m_currentTranslator = new QTranslator();
    bool loaded = false;

    // Jeśli przycisk ma tekst "Polish" (czyli aktualnie aplikacja działa po angielsku),
    // ładujemy tłumaczenie angielskie, aby program wyświetlał angielski interfejs.
    if (m_button->text() == "Polish") {
        loaded = m_currentTranslator->load(":/languages/moj_projekt_en_150.qm");
        if (loaded) {
            qApp->installTranslator(m_currentTranslator);
            m_button->setIcon(QIcon(":/images/uk_flag.png"));
            m_button->setText("English");
        } else {
            qDebug() << "Nie udało się załadować tłumaczenia angielskiego.";
            delete m_currentTranslator;
            m_currentTranslator = nullptr;
        }
    } else {
        // W przeciwnym przypadku przycisk wskazuje "English" (aplikacja działa po angielsku),
        // więc przełączamy na polskie tłumaczenie.
        loaded = m_currentTranslator->load(":/languages/moj_projekt_pl_150.qm");
        if (loaded) {
            qApp->installTranslator(m_currentTranslator);
            m_button->setIcon(QIcon(":/images/poland_flag.png"));
            m_button->setText("Polish");
        } else {
            qDebug() << "Nie udało się załadować tłumaczenia polskiego.";
            delete m_currentTranslator;
            m_currentTranslator = nullptr;
        }
    }
}
