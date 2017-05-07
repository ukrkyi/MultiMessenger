#include "messenger.h"
#include <QApplication>
#include <QLibraryInfo>
#include <QTranslator>

int main(int argc, char* argv[]) {
    QApplication a(argc, argv);

    QTranslator qtBaseTranslator;
    if (qtBaseTranslator.load("qtbase_" + QLocale::system().name(),
            QLibraryInfo::location(QLibraryInfo::TranslationsPath))) {
        a.installTranslator(&qtBaseTranslator);
    }
    Messenger w;
    return a.exec();
}
