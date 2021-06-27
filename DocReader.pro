QT       += core gui printsupport  qml quick
QT       += webengine webenginewidgets
TARGET    = texteditor

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++11

OTHER_FILES +=\
    main.qml

SOURCES += \
    docreader.cpp \
    doctohtml.cpp \
    main.cpp

HEADERS += \
    docreader.h \
    doctohtml.h

RESOURCES += \
   res.qrc

RC_FILE += file.rc


# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

DISTFILES += \
    file.rc \
    img/Interface/boldText.png \
    img/Interface/centralAlign.png \
    img/Interface/italicText.png \
    img/Interface/leftAlign.png \
    img/Interface/rightAlign.png \
    img/Interface/underlineText.png \
    img/icon.ico

