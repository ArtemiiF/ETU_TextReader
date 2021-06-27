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

    qmlRegisterType<DocReader>("Document.Reader",1,0,"DocReader");

    QQmlApplicationEngine engine(QUrl(QStringLiteral("qrc:/main.qml")));
    if (engine.rootObjects().isEmpty())
        return -1;

    return a.exec();
}
