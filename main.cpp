#include "textreader1.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    TextReader w;

    w.show();

    return a.exec();
}
