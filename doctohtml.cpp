#include "doctohtml.h"

DocToHtml::DocToHtml()
{

}

 void DocToHtml::extractDoc(QString filePath)
 {


     QString dir = "C:\\tempFiles";
     QDir().mkdir(dir);

     QString zfolder(dir);
     QZipReader zip_r(filePath);


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
