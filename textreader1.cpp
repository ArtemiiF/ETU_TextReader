#include "textreader1.h"

TextReader::TextReader()
{
    openAction = new QAction(tr("&Open"),this);

    connect(openAction, SIGNAL(triggered()), this, SLOT(open()));

    fileMenu = menuBar()->addMenu(tr("&File"));
    fileMenu->addAction(openAction);
    fileMenu->addSeparator();

    setWindowTitle(tr("DocReader"));
}

void TextReader::open()
{
    //Получаем адрес файла
    QString fileName = QFileDialog::getOpenFileName(this, tr("Open File"), "",
        tr("Docx,Odt (*.docx *.odt)"));

    if (fileName != "")
    {
        //Если формат docx
        if(fileName.endsWith(".docx"))
        {
            axw = new QAxWidget("Word.Document",nullptr);
            axw->setControl(fileName);
            setCentralWidget(axw);
            axw->setGeometry(0,0,800,600);
            axw->show();

        }
    }
}




