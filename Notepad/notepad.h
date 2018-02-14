#ifndef NOTEPAD_H
#define NOTEPAD_H

#include <QMainWindow>
#include <QFileDialog>
#include <QMessageBox>
#include <QFile>
#include <QTextStream>
#include <QFontDialog>
#include <QCloseEvent>
#include <QLineEdit>
#include <QDialog>
#include <QPushButton>
#include <QLabel>
#include <QDesktopServices>
#include <QTextCursor>
#include <QDebug>
#include <QTextEdit>
#include <QPainter>
#include <QPlainTextEdit>
#include <QClipboard>

#include "highlighter.h"

namespace Ui {
class Notepad;
}


class Notepad : public QMainWindow
{
    Q_OBJECT

public:
    explicit Notepad(QWidget *parent = 0);
    ~Notepad();



protected:
    void openFindReplaceDialog(QString flag);
    void showStatusBar();


private slots:
    void highlightCurrentLine();
    void on_actionNew_triggered();

    void on_actionOpen_triggered();

    void on_actionSave_triggered();

    void on_actionSave_as_triggered();

    void on_actionFont_triggered();

    void on_actionExit_triggered();

    void on_actionUndo_triggered();

    void on_actionCut_triggered();

    void on_actionCopy_triggered();

    void on_actionPaste_triggered();

    void on_actionDelete_triggered();

    void on_actionFind_triggered();

    void on_actionMD5_triggered();

    void on_actionBlog_triggered();

    void on_actionBase64_Encode_triggered();

    void on_actionBase64_Decode_triggered();

    void on_actionURL_Encode_triggered();

    void on_actionURL_Decode_triggered();

    void on_actionConvert_to_Upper_triggered();

    void on_actionConver_to_Lower_triggered();

    void on_actionFirst_Letter_Upper_triggered();

    void on_actionConvert_UL_triggered();

    void on_actionReplace_triggered();

    void updateMenuActionStatus();
private:
    Ui::Notepad *ui;
    QString CurrentFile;
    bool isUntitled;
    bool hasSaved;

    // 状态栏
    QLabel *statusLabel;

     QClipboard *clip;

    //语法高亮
    Highlighter *highlighter;

    bool loadFile(const QString &fileName);
    bool saveFile(const QString &fileName);
    bool saveFileAs();
    bool save();
    bool saveBeforeAction();
    void closeEvent(QCloseEvent *event); // 关闭事件

    void initStatus();
};

#endif // NOTEPAD_H
