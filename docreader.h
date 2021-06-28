#ifndef DOCREADER_H
#define DOCREADER_H

#include <QQuickTextDocument>
#include <QtGui/QTextCharFormat>
#include <QtCore/QTextCodec>
#include <qqmlfile.h>
#include <QTextDocument>
#include <QFontDatabase>
#include <QtGui/QTextDocument>
#include <QtGui/QTextCursor>
#include <QtGui/QFontDatabase>
#include <QtCore/QFileInfo>
#include <QtWebEngine/QtWebEngine>
#include <QtWebEngineWidgets/QWebEngineView>
#include <QtPrintSupport/QPrinter>
#include <QtSerialPort/QSerialPort>
#include <functional>

class DocReader: public QObject
{
    Q_OBJECT;
    Q_ENUMS(HAlignment)

    //Связь с интерфейсом
    Q_PROPERTY(QQuickItem *target READ getTarget WRITE setTarget NOTIFY targetChanged)
    Q_PROPERTY(int cursorPosition READ getCursorPosition WRITE setCursorPosition NOTIFY cursorPositionChanged)
    Q_PROPERTY(int selectionStart READ getSelectionStart WRITE setSelectionStart NOTIFY selectionStartChanged)
    Q_PROPERTY(int selectionEnd READ getSelectionEnd WRITE setSelectionEnd NOTIFY selectionEndChanged)
    Q_PROPERTY(QColor textColor READ textColor WRITE setTextColor NOTIFY textColorChanged)
    Q_PROPERTY(QString fontFamily READ getFontFamily WRITE setFontFamily NOTIFY fontFamilyChanged)

    Q_PROPERTY(bool bold READ bold WRITE setBold NOTIFY boldChanged)
    Q_PROPERTY(bool italic READ italic WRITE setItalic NOTIFY italicChanged)
    Q_PROPERTY(bool underline READ underline WRITE setUnderline NOTIFY underlineChanged)

    Q_PROPERTY(int fontSize READ getFontSize WRITE setFontSize NOTIFY fontSizeChanged)

    Q_PROPERTY(QStringList defaultFontSizes READ defaultFontSizes NOTIFY defaultFontSizesChanged)

    Q_PROPERTY(QString filePath READ getFilePath WRITE setFilePath NOTIFY filePathChanged)
    Q_PROPERTY(QString text READ getText WRITE setText NOTIFY textChanged)
    Q_PROPERTY(QString docTitle READ getDocTitle WRITE setDocTitle NOTIFY docTitleChanged)

    Q_PROPERTY(Qt::Alignment alignment READ alignment WRITE setAlignment NOTIFY alignmentChanged)

public:
    DocReader();
    ~DocReader();
    QStringList defaultFontSizes() const;

    //Геттеры для инфы про файл
    QString getFilePath() const;
    QString getText() const;
    QString getDocTitle() const;

    //Инфа по тексту(на месте курсора)
    QString getFontFamily() const;
    QColor textColor() const;
    bool bold() const;
    bool italic() const;
    bool underline() const;
    int getFontSize() const;

    //Курсор
    void setCursorPosition(int position);
    void setSelectionStart(int position);
    void setSelectionEnd(int position);

    //Простые геттеры
    int getCursorPosition()const {return cursorPosition;}
    int getSelectionStart() const { return selectionStart; }
    int getSelectionEnd() const { return selectionEnd; }
    QQuickItem *getTarget(){return target;}

    //Текст справа по центру слева
    Qt::Alignment alignment() const;
    void setAlignment(Qt::Alignment a);

    //Доп функции
    void setTarget(QQuickItem *target);
  Q_INVOKABLE void convertToPdf(/*const QUrl &fileUrl, const QString &fileType*/);


//слоты
public Q_SLOTS:

        void setFilePath( const QString &filePath);
        void setText( const QString &text);
        void setDocTitle(QString &docTitle);

        //Слоты связанные с отображение текста(шрифт, размер, цвет и т.д.)
        void setBold(bool flag);
        void setItalic(bool flag);
        void setUnderline(bool flag);
        void setFontSize(int flag);
        void setTextColor(const  QColor &textColor);
        void setFontFamily(const QString &fontFamily);


Q_SIGNALS:
        void textChanged();
        void docTitleChanged();
        void filePathChanged();

        void targetChanged();
        void cursorPositionChanged();
        void selectionStartChanged();
        void selectionEndChanged();
        void textColorChanged();
        void fontFamilyChanged();
        void fontSizeChanged();
        void defaultFontSizesChanged();

        void boldChanged();
        void italicChanged();
        void underlineChanged();

        void alignmentChanged();

        void error(QString message);

private:
        //Основная информация о документе(путь, текст, название)
        QString filePath;
        QString text;
        QString docTitle;

        //Шрифт
        int fontSize;
        QFont font;

        QQuickItem *target;

        QTextDocument *doc;

        //Курсор
        QTextCursor textCursor() const;
        int cursorPosition;
        int selectionStart;
        int selectionEnd;

        //
        void mergeFormatOnWordOrSelection(const QTextCharFormat &format);
        void reset();

};

#endif // DOCREADER_H
