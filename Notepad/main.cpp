#include "notepad.h"
#include <QApplication>
#include <QTranslator>


int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    QTranslator translator;
    translator.load(":/language/NotepadI18N_zh_CN.qm");
    a.installTranslator(&translator);
    Notepad w;
    w.resize(640, 512);
    w.show();
    return a.exec();
}
