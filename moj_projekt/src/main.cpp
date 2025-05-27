#include "mainwindow.hpp"
#include <QApplication>
#include <QLocale>
#include <QTranslator>

int main(int argc, char *argv[])
{
    // Tworzenie aplikacji Qt
    QApplication a(argc, argv);

    // Tworzenie obiektu tłumacza
    QTranslator translator;

    // Pobranie listy dostępnych języków interfejsu użytkownika z systemu
    const QStringList uiLanguages = QLocale::system().uiLanguages();

    // Iteracja po dostępnych językach
    for (const QString &locale : uiLanguages) {
        // Tworzenie nazwy pliku z tłumaczeniem na podstawie lokalizacji
        const QString baseName = "moj_projekt_" + QLocale(locale).name();
        if (translator.load(":/i18n/" + baseName)) {
            a.installTranslator(&translator);
            break;
        }
    }
    // Tworzenie i wyświetlanie głównego okna aplikacji
    MainWindow w;
    w.show();
    // Uruchomienie głównej pętli aplikacji
    return a.exec();
}
