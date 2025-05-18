#include "translate.hpp"
#include <QIcon>

Translator::Translator(QPushButton* button)
    : m_button(button)
{
    // Inicjalizacja przycisku (aplikacja działa po angielsku)
    m_button->setIcon(QIcon(":/images/poland_flag.png"));
    m_button->setText("Polish");
}

void Translator::toggleLanguage()
{
    if (m_button->text() == "Polish") {
        // Zmiana ikony i tekstu
        m_button->setIcon(QIcon(":/images/uk_flag.png"));
        m_button->setText("English");
    } else {
        // Zmiana ikony i tekstu
        m_button->setIcon(QIcon(":/images/poland_flag.png"));
        m_button->setText("Polish");
    }
}
