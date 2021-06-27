#include "docreader.h"
#include <QApplication>
#include <QtQml/QQmlApplicationEngine>
#include <QtWidgets/QApplication>
#include <QtGui/QGuiApplication>




int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    a.setOrganizationName("Etu Students");
    a.setOrganizationDomain("Etustudents.com");
    a.setApplicationName("DocReader");
    qDebug()<<argc;
    qDebug()<<argv[0];

    QDir().mkdir("tempFiles");

    //Загрузка аргумента как пути к файлу
    if(argc==2)
    {
        QString str = argv[1];
        QFile file ("tempFiles/initpath.txt");

        if(file.open(QIODevice::ReadWrite))
        {
            QTextStream stream (&file);
            stream<<str;
        }
    }

    qmlRegisterType<DocReader>("Document.Reader",1,0,"DocReader");
    QQmlApplicationEngine engine(QUrl(QStringLiteral("qrc:/main.qml")));
    if (engine.rootObjects().isEmpty())
        return -1;

    QFile file("tempFiles/initPath.txt");
        if(file.open(QIODevice::WriteOnly | QIODevice::Truncate))
                file.close();

    return a.exec();
}
