#ifndef TEXTREADER1_H
#define TEXTREADER1_H

#include <QWidget>
#include <QtGui>
#include <QMenu>
#include <QMenuBar>
#include <QTextEdit>
//#include <QVBoxLayout>
#include <QToolBar>
#include <QMainWindow>
#include <QFileDialog>
#include <QMessageBox>
#include <QAxObject>
#include <QAxWidget>
#include <QString>


class TextReader : public QMainWindow
{
    Q_OBJECT;

public:
    TextReader();

private slots:
    void open();

private:
    QTextEdit *textEdit;
    QAction *openAction;
    QAxWidget *axw;

    QMenu *fileMenu;
};

#endif // TEXTREADER1_H
