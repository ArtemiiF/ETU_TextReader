#include "docreader.h"
#include <QFileDialog>
#include "doctohtml.h"
#include <QXmlQuery>

DocReader::DocReader()
{
    target = 0;
    doc = 0;
    cursorPosition = -1;
    selectionEnd = 0;
    selectionStart = 0;
}

DocReader::~DocReader()
{
}

void DocReader::setTarget(QQuickItem *target)
{
    doc = 0;
    this->target = target;
    if (!(this->target))
        return;

    QVariant document = this->target->property("textDocument");
    if (document.canConvert<QQuickTextDocument*>())
    {
        QQuickTextDocument *qqdoc = document.value<QQuickTextDocument*>();
        if (qqdoc)
            doc = qqdoc->textDocument();
    }
    emit targetChanged();
}

//Доделать как влад пришлет свою часть
//геттер и сеттер для filePath
void DocReader::setFilePath(const QString &filePath)
{
    QString dir = "C:\\tempFiles";
    if(this->filePath!=filePath)
    {
        this->filePath = filePath;

        QString fp = QQmlFile::urlToLocalFileOrQrc(filePath);

        if(filePath.endsWith(".html"))
        {
            if(QFile::exists(fp))
                    {
                        QFile file(fp);
                        if(file.open(QFile::ReadOnly))
                        {
                            QByteArray data = file.readAll();
                            QTextCodec *codec = QTextCodec::codecForHtml(data);
                            setText(codec->toUnicode(data));

                            if(doc)
                                doc->setModified(false);
                            if(filePath.isEmpty())
                                docTitle = QStringLiteral("untitled.txt");
                            else
                                docTitle = QFileInfo(filePath).fileName();

                            emit textChanged();
                            emit docTitleChanged();

                            reset();
                        }
                    }
                    emit filePathChanged();
            return;

        }

        if(QFile::exists(fp))
        {

        DocToHtml doctothml;

        doctothml.extractDoc(fp);

        QXmlQuery q(QXmlQuery::XSLT20);

        QFile filexml(dir+"\\word\\document.xml");

        if (!filexml.open(QIODevice::ReadOnly))
        {
                return;
            }

        QByteArray bArray;

            while (!filexml.atEnd())
            {
                bArray += filexml.readLine();
            }

            QBuffer xOriginalContent(&bArray);
                xOriginalContent.open(QBuffer::ReadWrite);
                xOriginalContent.reset();

                if (q.setFocus(&xOriginalContent))
                         qDebug() << "File Loaded";

                QFile filexsl("temp.xsl");

                if (!filexsl.open(QIODevice::ReadOnly))
                {
                        return;
                    }

                QByteArray aArray;

                    while (!filexsl.atEnd())
                    {
                        aArray += filexsl.readLine();
                    }

                    QBuffer yOriginalContent(&aArray);
                        yOriginalContent.open(QBuffer::ReadWrite);
                        yOriginalContent.reset();

                        q.setQuery(&yOriginalContent);

                        QString out;
                        q.evaluateTo(&out);
                        qDebug()<<out;



        //Создание html
        QFile file(dir+"\\example.html");
        if (file.open(QIODevice::ReadWrite))
        {
                QTextStream stream(&file);
                stream << out;



        //Открываем html

                QByteArray data = file.readAll();
                QTextCodec *codec = QTextCodec::codecForHtml(data);
                setText(codec->toUnicode(data));

                if(doc)
                    doc->setModified(false);
                if(filePath.isEmpty())
                    docTitle = QStringLiteral("untitled.txt");
                else
                    docTitle = QFileInfo(filePath).fileName();

                emit textChanged();
                emit docTitleChanged();

                reset();
            }
        }
        emit filePathChanged();
    }
}

QString DocReader::getFilePath() const
{
    return filePath;
}

//Конвертирование в pdf
void DocReader::convertToPdf(/*const QUrl &fileUrl, const QString &fileType*/)
{

/*    const QString pdfPath = fileUrl.toLocalFile();
    QString ending = fileType;
    const QUrl currPath = filePath;

    if(!pdfPath.endsWith(ending))
       pdfPath += ending;

    QFile file(pdfPath);

    if(!file.open(QFile::WriteOnly|QFile::Truncate))
    {
        emit error (tr("Cannot Save:")+file.errorString());
        return;
    }

    //Переделать не работает
    qDebug()<<currPath;
    qDebug()<<pdfPath;

    QWebEngineView* webView = new QWebEngineView();
    webView->load(currPath);
    webView->page()->printToPdf(pdfPath);*/


    QWebEngineView* webView = new QWebEngineView();


    const QString fp = QFileDialog::getOpenFileName(0, QObject::tr("Открыть документ"),"D:\\", QObject::tr("Файлы документов (*.html)"));
    QFileInfo fileInfo(fp);

    webView->load(QUrl::fromLocalFile(fileInfo.filePath()));
    const QString pdf = QFileDialog::getSaveFileName(0, "Ep", "D:\\", "pd (*.pdf)");
    webView->page()->printToPdf(pdf);

}

//Геттер и сеттер для заголовка
QString DocReader::getDocTitle() const
{
    return docTitle;
}

void DocReader::setDocTitle(QString &title)
{
    if(docTitle!=title)
    {
        docTitle = title;
        emit docTitleChanged();
    }
}

//Геттер и сеттер для текста
QString DocReader::getText() const
{
    return text;
}

void DocReader::setText(const QString &text)
{
    if(this->text!=text)
    {
        this->text=text;
        emit textChanged();
    }
}

//Курсор
void DocReader::setCursorPosition(int position)
{
    if(position == cursorPosition)
    {
        return;
    }
    cursorPosition = position;

    reset();
}

QTextCursor DocReader::textCursor() const
{
    if(!doc)
        return QTextCursor();

    QTextCursor cursor = QTextCursor(doc);
    /*
    if(начало выделение != конец выделения)
        ...
    else*/
    if (selectionStart != selectionEnd)
    {
        cursor.setPosition(selectionStart);
        cursor.setPosition(selectionEnd, QTextCursor::KeepAnchor);
    }
    else
    {
        cursor.setPosition(cursorPosition);
    }
    //cursor.setPosition(cursorPosition);

    return cursor;
}

void DocReader::mergeFormatOnWordOrSelection(const QTextCharFormat &format)
{
    QTextCursor cursor = textCursor();
    if(!cursor.hasSelection())
    {
        cursor.select(QTextCursor::WordUnderCursor);
    }
    cursor.mergeCharFormat(format);
}

void DocReader::setSelectionStart(int position)
{
    selectionStart = position;
}
void DocReader::setSelectionEnd(int position)
{
    selectionEnd = position;
}

//Тип шрифта(косой, полужирный, подчеркнутый)
//Геттеры сеттеры
bool DocReader::bold()const
{
    QTextCursor cursor = textCursor();
    if(cursor.isNull())
        return false;
    return textCursor().charFormat().fontWeight() == QFont::Bold;
}

bool DocReader::italic()const
{
    QTextCursor cursor = textCursor();
    if (cursor.isNull())
        return false;
    return textCursor().charFormat().fontItalic();
}

bool DocReader::underline()const
{
    QTextCursor cursor = textCursor();
    if (cursor.isNull())
        return false;
    return textCursor().charFormat().fontUnderline();
}

void DocReader::setBold(bool flag)
{
    QTextCharFormat format;
    format.setFontWeight(flag ? QFont::Bold:QFont::Normal);
    mergeFormatOnWordOrSelection(format);
    emit boldChanged();
}

void DocReader::setItalic(bool flag)
{
    QTextCharFormat format;
    format.setFontItalic(flag);
    mergeFormatOnWordOrSelection(format);
    emit italicChanged();
}

void DocReader::setUnderline(bool flag)
{
    QTextCharFormat format;
    format.setFontUnderline(flag);
    mergeFormatOnWordOrSelection(format);
    emit underlineChanged();
}

//Размер шрифта
//Геттеры сеттеры
int DocReader::getFontSize() const
{
    QTextCursor cursor = textCursor();
    if(cursor.isNull())
    {
        return 0;
    }
    QTextCharFormat format = cursor.charFormat();
    return format.font().pointSize();
}

void DocReader::setFontSize(int size)
{
    QTextCursor cursor = textCursor();
    if (cursor.isNull())
        return;
    QTextCharFormat format;
    format.setFontPointSize(size);
    mergeFormatOnWordOrSelection(format);
    emit fontSizeChanged();
}

//Вид шрифта
QString DocReader::getFontFamily() const
{
    QTextCursor cursor = textCursor();
    if (cursor.isNull())
        return QString();
    QTextCharFormat format = cursor.charFormat();
    return format.font().family();
}

void DocReader::setFontFamily(const QString &font)
{
    QTextCursor cursor = textCursor();
    if (cursor.isNull())
        return;
    QTextCharFormat format;
    format.setFontFamily(font);
    mergeFormatOnWordOrSelection(format);
    emit fontFamilyChanged();
}

QStringList DocReader::defaultFontSizes() const
{
    QStringList sizes;
    QFontDatabase db;
    foreach (int size, db.standardSizes())
        sizes.append(QString::number(size));
    return sizes;
}
//Цвет под курсором
QColor DocReader::textColor() const
{
    QTextCursor cursor = textCursor();
    if (cursor.isNull())
        return QColor(Qt::black);
    QTextCharFormat format = cursor.charFormat();
    return format.foreground().color();
}

void DocReader::setTextColor(const QColor &color)
{
    QTextCursor cursor = textCursor();
    if (cursor.isNull())
        return;
    QTextCharFormat format;
    format.setForeground(QBrush(color));
    mergeFormatOnWordOrSelection(format);

    emit textColorChanged();
}

//Центрирование текста
Qt::Alignment DocReader::alignment() const
{
    QTextCursor cursor = textCursor();
    if (cursor.isNull())
        return Qt::AlignLeft;
    return textCursor().blockFormat().alignment();
}

void DocReader::setAlignment(Qt::Alignment a)
{
    if (!doc)
        return;

    QTextBlockFormat format;
    format.setAlignment((Qt::Alignment) a);
    QTextCursor cursor = QTextCursor(doc);
    cursor.setPosition(selectionStart, QTextCursor::MoveAnchor);
    cursor.setPosition(selectionEnd, QTextCursor::KeepAnchor);
    cursor.mergeBlockFormat(format);

    emit alignmentChanged();
}

//Перезагрузка инфы
void DocReader::reset()
{
    emit fontFamilyChanged();
    emit boldChanged();
    emit italicChanged();
    emit underlineChanged();
    emit fontSizeChanged();
    emit textColorChanged();
}
