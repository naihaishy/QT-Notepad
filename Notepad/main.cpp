#include "notepad.h"
#include <QApplication>
#include <QTranslator>
#include "commonhelper.h"


int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    // 翻译
    QTranslator translator;
    translator.load(":/language/NotepadI18N_zh_CN.qm");
    a.installTranslator(&translator);

    if (!QSystemTrayIcon::isSystemTrayAvailable()) {
        QMessageBox::critical(0, QObject::tr("Systray"), QObject::tr("I couldn't detect any system tray on this system."));
        return 1;
    }


    //QApplication::setQuitOnLastWindowClosed(false);

    // 主窗口
    Notepad w;
    w.show();
    return a.exec();
}
