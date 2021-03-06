#include "docreader.h"
#include <QFileDialog>
#include "doctohtml.h"
#include <QDir>

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
    if(this->filePath!=filePath)
    {
        if(!QFile::exists(QUrl(filePath).toLocalFile()))
        {
            emit error(tr("Ошибка в выборе файла. Его нет :-("));
            return;
        }

        this->filePath = filePath;

        QString fp = "tempDoc.html";

        DocToHtml::convertToHtml(filePath);

        if(fp.endsWith(".html"))
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
                        file.close();
                    }
                    emit filePathChanged();

        }



    }
}

QString DocReader::getFilePath() const
{
    return filePath;
}

//Конвертирование в pdf(Не работает)
void DocReader::convertToPdf()
{

    /*const QString pdfPath = fileUrl.toLocalFile();

    QFile file(pdfPath);

    if(!file.open(QFile::WriteOnly|QFile::Truncate))
    {
        emit error (tr("Cannot convert:")+file.errorString());
        return;
    }*/

    QWebEngineView* webView = new QWebEngineView();

    /*const QFile fp("tempDoc.html");
    const QFileInfo fileInfo(fp);*/

    const QString fp = QFileDialog::getOpenFileName(0, QObject::tr("Открыть документ"),"D:\\", QObject::tr("Файлы документов (*.html)"));
        QFileInfo fileInfo(fp);

    //const QString fp1 = QFileDialog::getOpenFileName(0, QObject::tr("Открыть документ"),"D:\\", QObject::tr("Файлы документов (*.html)"));
    //QFileInfo fileInfo1(fp1);
    const QString pdf = QFileDialog::getSaveFileName(0, "Ep", "D:\\", "pd (*.pdf)");


    //qDebug()<<"1-"+fileInfo.absoluteFilePath();
   // qDebug()<<"1-"+fileInfo1.absoluteFilePath();
    //qDebug()<<"2-"+pdfPath;
    //qDebug()<<"2-"+pdf;


    webView->load(QUrl::fromLocalFile(fileInfo.filePath()));
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
