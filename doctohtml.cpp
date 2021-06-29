#include "doctohtml.h"
#include <QTextCodec>

DocToHtml::DocToHtml()
{

}

 void DocToHtml::extractDoc(QUrl filePath)
 {
     QString dir = "C:\\DocReaderTempFiles\\";
     QDir().mkdir(dir);

     QString zfolder(dir);
     QZipReader zip_r(filePath.toLocalFile());

     qDebug()<<filePath;

     QDir baseDir(zfolder);
     QVector<QZipReader::FileInfo> allFiles = zip_r.fileInfoList();

     foreach (QZipReader::FileInfo fi, allFiles)
     {
             const QString absPath = zfolder + QDir::separator() + fi.filePath;
             if (fi.isDir)
             {
                 if (!baseDir.mkpath(absPath)){};
     //                return false;
                 if (!QFile::setPermissions(absPath, fi.permissions)){};
     //               return false;
             }
      }

     foreach (QZipReader::FileInfo fi, allFiles)
     {
             const QString absPath = zfolder + "/" + fi.filePath;
             QString d;
             if(absPath.indexOf("/") != -1)
             {
                 d = absPath.left(absPath.lastIndexOf("/"));
                 QDir dir(d);
                 if(!dir.exists())
                 {
                     dir.mkdir(dir.path());
                 }
             }
      }

     foreach (QZipReader::FileInfo fi, allFiles) {
             const QString absPath = zfolder + "/" + fi.filePath;
             if(absPath.endsWith("/"))
             {
     //            if( !baseDir.exists(fi.filePath) )
     //            {
                     if (!baseDir.mkdir(fi.filePath)){};
     //                    return false;
                     if (!QFile::setPermissions(absPath, fi.permissions)){};
     //                    return false;
     //            }
             }
             else
             {
                 QString d;
                 if(absPath.indexOf("/") != -1)
                 {
                     d = absPath.left(absPath.lastIndexOf("/"));
                     QDir dir(d);
                     if(!dir.exists()){
                         dir.mkdir(dir.path());
                     }
                 }
                 QFile f(absPath);
                 if (!f.open(QIODevice::WriteOnly))
     //                return false;
                 f.write(zip_r.fileData(fi.filePath));
                 f.setPermissions(fi.permissions);
                 f.close();
             }
         }
     //    return true;
         //  ------------------------
         // set up symlinks
         foreach (QZipReader::FileInfo fi, allFiles)
         {
             const QString absPath = zfolder + QDir::separator() + fi.filePath;
             if (fi.isSymLink) {
                 QString destination = QFile::decodeName(zip_r.fileData(fi.filePath));
                 if (destination.isEmpty())
     //                return false;
                 QFileInfo linkFi(absPath);
     //            if (!QFile::exists(linkFi.absolutePath()))
     //                QDir::root().mkpath(linkFi.absolutePath());
                 if (!QFile::link(destination, absPath)){};
     //                return false;
             }
         }

         zip_r.extractAll(dir);
 }

 void DocToHtml::convertToHtml(QString filePath)
 {
         extractDoc(filePath);

         QFile inp("C:\\DocReaderTempFiles\\word\\document.xml");
         inp.open(QIODevice::ReadOnly|QIODevice::Text);
         QByteArray be = NULL;
          be.append(inp.readAll());
          QBuffer xml(&be);
          xml.open(QIODevice::ReadOnly);

          QFile inp1(":/temp.xsl");
          inp1.open(QIODevice::ReadOnly|QIODevice::Text);
          QByteArray be1 = NULL;
           be1.append(inp1.readAll());
           QBuffer xsl(&be1);
           xsl.open(QIODevice::ReadOnly);

         QXmlQuery q(QXmlQuery::XSLT20);

         q.setFocus(&xml);
         q.setQuery(&xsl);
         QString out;

         q.evaluateTo(&out);
         qDebug() << out;

         deleteGarbage();
         creationHtml(out);
 }

 void DocToHtml::deleteGarbage()
 {
     qDebug()<<"delete temp files";
     QDir dir("C:\\DocReaderTempFiles");
     dir.removeRecursively();
 }

 void DocToHtml::creationHtml(QString htmlPage)
 {
     QString fileName = "tempDoc.html";

     if(QFile::exists(fileName))
     {
        QFile file(fileName);
        file.remove();
     }

    QFile file(fileName);

    if (file.open(QIODevice::ReadWrite | QIODevice::Text))
    {

        QTextStream stream( &file );
            //stream.setString(&htmlPage);
        stream.setCodec("UTF-8");
        stream<<htmlPage;


    }
 }
