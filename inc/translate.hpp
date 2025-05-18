#ifndef TRANSLATE_HPP
#define TRANSLATE_HPP
#include <QPushButton>

class Translator
{
public:
    Translator(QPushButton* button);

    void toggleLanguage();

private:
    QPushButton* m_button;
};

#endif // TRANSLATE_HPP
