#include "notepad.h"
#include "ui_notepad.h"
#include "md5dialog.h"
#include "finddialog.h"


Notepad::Notepad(QWidget *parent) : QMainWindow(parent), ui(new Ui::Notepad)
{
    ui->setupUi(this);
    // 设置字体信息
    ui->textEdit->setFont(QFont(tr("Consolas"), 14));
    this->setCentralWidget(ui->textEdit);

    // 初始化文件为未保存状态
    isUntitled = true;
    hasSaved = false;
    CurrentFile = "Untitled.txt";

    //语法高亮
    highlighter = new Highlighter(ui->textEdit->document());


    // 状态栏
    // 正常状态信息
    statusLabel = new QLabel();
    statusLabel->setMinimumSize(150,20);
    statusLabel->setFrameShape(QFrame::WinPanel);
    statusLabel->setFrameShadow(QFrame::Sunken);
    statusLabel->setText(tr("Hello"));
    ui->statusBar->addWidget(statusLabel);
    // 永久状态信息
    QLabel *permanentLabel = new QLabel(this);
    permanentLabel->setFrameStyle(QFrame::Box | QFrame::Sunken);
    permanentLabel->setText(tr("<a href=\"http://www.zhfsky.com\">zhfsky.com</a>"));
    permanentLabel->setTextFormat(Qt::RichText);
    permanentLabel->setOpenExternalLinks(true);
    ui->statusBar->addPermanentWidget(permanentLabel);

}

Notepad::~Notepad()
{
    delete ui;
}

// 加载文件
bool Notepad::loadFile(const QString &fileName)
{
    QFile file(fileName); // 新建QFile对象
    if (!file.open(QFile::ReadOnly | QFile::Text))
    {
       QMessageBox::warning(this, tr("Notepad"), tr("Can't read file %1:\n%2.").arg(fileName).arg(file.errorString()));
       return false; // 只读方式打开文件，出错则提示，并返回false
    }
    QTextStream in(&file); // 新建文本流对象
    // 设置鼠标状态
    QApplication::setOverrideCursor(Qt::WaitCursor);
    // 读取文件的全部文本内容，并添加到编辑器中
    ui->textEdit->setPlainText(in.readAll());
    // 恢复鼠标状态
    QApplication::restoreOverrideCursor();
    // 设置当前文件
    CurrentFile = QFileInfo(fileName).canonicalFilePath();
    // 设置窗口标题
    setWindowTitle(CurrentFile);
    file.close();
    isUntitled = false;
    return true;
}

// 在执行action之前检测当前文档是否已经保存
bool Notepad::saveBeforeAction()
{
    // 当前文档被更改
    if(ui->textEdit->document()->isModified() && !hasSaved)
    {
        QMessageBox box(this);
        box.setWindowTitle(tr("Warnging"));
        box.setIcon(QMessageBox::Warning);
        box.setText(tr("Save the changes to\r") + CurrentFile + "?");
        QPushButton *yesBtn = box.addButton(tr("Yes"), QMessageBox::YesRole);
        QPushButton *noBtn = box.addButton(tr("No"), QMessageBox::NoRole);
        QPushButton *cancelBtn = box.addButton(tr("Cancel"), QMessageBox::RejectRole);
        box.exec();
        QPushButton* clickedButton =(QPushButton*)box.clickedButton();
        if ( clickedButton== yesBtn)
            return save(); // 保存
        else if (clickedButton == noBtn)
            return true; //不保存 直接返回true
        else if (clickedButton == cancelBtn)
            return false;// 什么也不做
    }
    // 如果文档没有被更改，则直接返回true
    return true;
}

// 保存文件
bool Notepad::save()
{
    if(isUntitled) // 文档以前没有保存过
    {
        return saveFileAs();
    }else{
        return saveFile(CurrentFile);
    }
}

bool Notepad::saveFileAs()
{
    QString fileName = QFileDialog::getSaveFileName(this, tr("Save as"), CurrentFile, tr("Document(*.txt)"));
    if(fileName.isEmpty())
    {
        return false;
    }else{
        return saveFile(fileName);
    }
}

bool Notepad::saveFile(const QString &fileName)
{
    QFile file(fileName);
    if (!file.open(QIODevice::WriteOnly | QFile::Text))
    {
        QMessageBox::warning(this, tr("Notepad"), tr("Cant't write file %1：/n %2").arg(fileName).arg(file.errorString()));
        return false;
    }
    QTextStream out(&file);
    // 鼠标指针变为等待状态
    QApplication::setOverrideCursor(Qt::WaitCursor);
    QString text = ui->textEdit->toPlainText();
    out << text;
    file.flush();
    file.close();
    // 鼠标指针恢复原来的状态
    QApplication::restoreOverrideCursor();
    isUntitled = false;
    // 获得文件的标准路径
    CurrentFile = QFileInfo(fileName).canonicalFilePath();
    // 设置窗口标题
    setWindowTitle(CurrentFile);
    hasSaved = true;
    return true;

}

// 直接关闭窗口
void Notepad::closeEvent(QCloseEvent *event)
{
    if(saveBeforeAction())
    {
        event->accept();
    }else{
        event->ignore();
    }
}


void Notepad::on_actionNew_triggered()
{
    if(saveBeforeAction())
    {
        CurrentFile = "Untitled.txt";
        // 设置窗口标题
        setWindowTitle(CurrentFile);
        ui->textEdit->setText("");
        // ui->textEdit->clear();
    }


}

void Notepad::on_actionOpen_triggered()
{
    if(saveBeforeAction())
    {
        QString fileName = QFileDialog::getOpenFileName(this, tr("Open the file"), tr(""), tr("Document(*.txt)"));
        if(!fileName.isEmpty())
        {
            loadFile(fileName);
        }
    }
}

void Notepad::on_actionSave_triggered()
{
    save();
}


void Notepad::on_actionSave_as_triggered()
{
    saveFileAs();
}

void Notepad::on_actionFont_triggered()
{
    bool fontSelected;
    QFont font = QFontDialog::getFont(&fontSelected, this);
    if (fontSelected) {
        ui->textEdit->setFont(font);
    }
}

void Notepad::on_actionExit_triggered()
{
    if(saveBeforeAction()){
        // qApp是指向应用程序的全局指针
        qApp->quit();
    }
}

void Notepad::on_actionUndo_triggered()
{
    ui->textEdit->undo();
}

void Notepad::on_actionCut_triggered()
{
    ui->textEdit->cut();
}

void Notepad::on_actionCopy_triggered()
{
    ui->textEdit->copy();
}

void Notepad::on_actionPaste_triggered()
{
    ui->textEdit->paste();
}

void Notepad::on_actionDelete_triggered()
{
    ui->textEdit->cut();
}

void Notepad::openFindReplaceDialog(QString flag)
{
    FindDialog *dlg = new FindDialog(this, flag);
    dlg->docmainEdit = ui->textEdit;
    dlg->show();
}

void Notepad::on_actionFind_triggered()
{
    openFindReplaceDialog("find");
}

void Notepad::on_actionReplace_triggered()
{
    openFindReplaceDialog("replace");
}

void Notepad::on_actionMD5_triggered()
{
    MD5Dialog *dlg = new MD5Dialog(this);
    dlg->show();
}

void Notepad::on_actionBlog_triggered()
{
    QDesktopServices::openUrl(QUrl(tr("http://www.zhfsky.com")));
}

void Notepad::on_actionBase64_Encode_triggered()
{
    QByteArray input;
    input.append(ui->textEdit->toPlainText());
    QString output(input.toBase64());
    ui->textEdit->setText(output);
}

void Notepad::on_actionBase64_Decode_triggered()
{
    QByteArray input;
    input.append(ui->textEdit->toPlainText());
    QString output(QByteArray::fromBase64(input));
    ui->textEdit->setText(output);
}

void Notepad::on_actionURL_Encode_triggered()
{

}

void Notepad::on_actionURL_Decode_triggered()
{

}

void Notepad::on_actionConvert_to_Upper_triggered()
{
    // 当前cursor对象
    QTextCursor cursor = ui->textEdit->textCursor();
    // 获取选择的文本
    QString text = cursor.selectedText();
    // 将选中文本全部转换为大写
    text = text.toUpper();
    // 删除选择的文本
    cursor.removeSelectedText();
    // 插入大写后的文本
    cursor.insertText(text);
    // 设置光标对象
    ui->textEdit->setTextCursor(cursor);

}

void Notepad::on_actionConver_to_Lower_triggered()
{
    // 当前cursor对象
    QTextCursor cursor = ui->textEdit->textCursor();
    // 获取选择的文本
    QString text = cursor.selectedText();
    // 将选中文本全部转换为小写
    text = text.toLower();
    // 删除选择的文本
    cursor.removeSelectedText();
    // 插入小写后的文本
    cursor.insertText(text);
    // 设置光标对象
    ui->textEdit->setTextCursor(cursor);

}

void Notepad::on_actionFirst_Letter_Upper_triggered()
{
    // 当前cursor对象
    QTextCursor cursor = ui->textEdit->textCursor();
    // 获取选择的文本
    QString text = cursor.selectedText();
    // 将选中文本全部转换为小写
    text = text.toLower();
    // 将选中文本的每个单词的首字母转换为大写
    QStringList parts = text.split(QRegExp("\\b")); // 每个Item 包括 单词 空白 符号
    for (int i = 0; i < parts.size(); ++i)
    {
        QString item = parts[i];
        if(!item.isEmpty()){
            parts[i].replace(0, 1, parts[i][0].toUpper());// 首字母大写
        }
    }

    QString output = parts.join("");// 拼接
    // 删除选择的文本
    cursor.removeSelectedText();
    // 插入大写后的文本
    cursor.insertText(output);
    // 设置光标对象
    ui->textEdit->setTextCursor(cursor);
}

void Notepad::on_actionConvert_UL_triggered()
{
    // 当前cursor对象
    QTextCursor cursor = ui->textEdit->textCursor();
    // 获取选择的文本
    QString text = cursor.selectedText();

    for(int i=0; i<text.size();i++){
        // 选中英语字母
        if(text[i].isLetter()){
            if(text[i].isLower()){
                // 该字母小写 将其转换为大写
                text[i] = text[i].toUpper();
            }else if(text[i].isUpper()){
                // 该字母大写 将其转换为小写
                text[i] = text[i].toLower();
            }

        }

    }
    // 删除选择的文本
    cursor.removeSelectedText();
    // 插入大写后的文本
    cursor.insertText(text);
    // 设置光标对象
    ui->textEdit->setTextCursor(cursor);
}


