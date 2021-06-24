QT       += core gui
QT       += qml quick
TARGET    = texteditor

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++11

RC_FILE += file.rc
OTHER_FILES +=\
    file.rc\
    main.qml

SOURCES += \
    docreader.cpp \
    main.cpp

HEADERS += \
    docreader.h

RESOURCES += \
   res.qrc


# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

DISTFILES += \
    img/Interface/boldText.png \
    img/Interface/centralAlign.png \
    img/Interface/italicText.png \
    img/Interface/leftAlign.png \
    img/Interface/rightAlign.png \
    img/Interface/underlineText.png \
    img/icon.ico

