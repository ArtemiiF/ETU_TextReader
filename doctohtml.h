#ifndef DOCTOHTML_H
#define DOCTOHTML_H

#include "QtGui\private\qzipreader_p.h"
#include "QtGui\private\qzipwriter_p.h"

#include <QDir>
#include <QDebug>
#include <QDirIterator>
#include <QFileDialog>

class DocToHtml
{
public:
    DocToHtml();


    void extractDoc(QString filePath);

};

#endif // DOCTOHTML_H
