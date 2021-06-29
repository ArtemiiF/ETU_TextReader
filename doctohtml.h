#ifndef DOCTOHTML_H
#define DOCTOHTML_H

#include "QtGui\private\qzipreader_p.h"
#include "QtGui\private\qzipwriter_p.h"
#include <QXmlQuery>

#include <QDir>
#include <QDebug>
#include <QDirIterator>
#include <QFileDialog>
#include <QBuffer>

class DocToHtml
{
public:
    DocToHtml();

    static void convertToHtml(QString filePath);

private:
    static void extractDoc(QUrl filePath);
    static void deleteGarbage();
    static void creationHtml(QString htmlPage);

};

#endif // DOCTOHTML_H
