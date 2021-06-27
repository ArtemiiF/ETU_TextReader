import QtQuick 2.2
import QtQuick.Controls 1.2
import QtQuick.Layouts 1.0
import QtQuick.Dialogs 1.1
import QtQuick.Window 2.1
import Document.Reader 1.0

ApplicationWindow
{
    width:640
    height:480

    minimumWidth: 600
    minimumHeight: 400

    visible: true

    title: doc.docTitle + " - DocReader"

    ColorDialog
    {
        id: colorDialog
        color: "black"
    }

    Action
    {
        id: alignLeftAction
        text: "&left"
        onTriggered: doc.alignment = Qt.AlignLeft
        checkable: true
        checked: doc.alignment == Qt.AlignLeft
        iconSource: "img/Interface/leftAlign.png"
        iconName: "text-to-left"
        shortcut: "ctrl+l"
    }

    Action
    {
        id: alignRightAction
        text: "&right"
        onTriggered: doc.alignment = Qt.AlignRight
        checkable: true
        checked: doc.alignment == Qt.AlignRight
        iconSource: "img/Interface/rightAlign.png"
        iconName: "text-to-right"
        shortcut: "ctrl+r"
    }

    Action
    {
        id: alignCentralAction
        text: "&left"
        onTriggered: doc.alignment = Qt.AlignCenter
        checkable: true
        checked: doc.alignment == Qt.AlignCenter
        iconSource: "img/Interface/centralAlign.png"
        iconName: "text-to-center"
    }

    Action
    {
        id: boldAction
        text: "&Bold"
        iconSource: "img/Interface/boldText.png"
        iconName: "text-to-bold"
        onTriggered: doc.bold = !doc.bold
        checkable: true
        checked: doc.bold
    }

    Action
    {
        id: italicAction
        text: "&Italic"
        iconSource: "img/Interface/italicText.png"
        iconName: "text-to-italic"
        onTriggered: doc.italic = !doc.italic
        checkable: true
        checked: doc.italic
    }

    Action
    {
        id: underlineAction
        text: "&Underline"
        iconSource: "img/Interface/underlineText.png"
        iconName: "text-to-underline"
        onTriggered: doc.underline = !doc.underline
        checkable: true
        checked: doc.underline
    }

    Action
    {
        id: fileOpenAction
        text:"Open"
        shortcut: "ctrl+o"
        onTriggered:
        {
            fileDialog.setNameFilters("HTML files (*.html)")
            fileDialog.type = 0;
            fileDialog.selectExisting = true;

            fileDialog.open()
        }

    }

    Action
    {
        id: fileConvertPdfAction
        text: "Convert file to PDF"
        onTriggered:
        {
            //fileDialog.setNameFilters("PDF file (*.pdf)")
            //fileDialog.type = 2;
            //fileDialog.selectExisting = false;
            doc.convertToPdf();
            //fileDialog.open();
        }
    }

    FileDialog
    {
        id: fileDialog
        //nameFilters: ["Docx, ODT files (*.docx,*.odt)"]
        nameFilters: ["HTML files (*.html, *.htm)"]
        property int type: 0    //0 - открыть, 1 - сохранить???, 2 - конвертация в pdf

        onAccepted:
        if(type == 0)
            doc.filePath = fileUrl
        else if(type == 1)
            doc.saveAs(fileUrl,selectNameFilter) //Потом добавить
        else if(type == 2)
            doc.convertToPdf(fileUrl,".pdf");
    }


    menuBar: MenuBar
    {
        Menu
        {
            title: "&File"
            MenuItem{action: fileOpenAction}
            MenuItem {action: fileConvertPdfAction}
        }
    }

    //Инфа по тексту под шрифтом
    toolBar: ToolBar
    {
        id: textInfoToolBar
        width: parent.width

        RowLayout
        {
            anchors.fill: parent
            spacing: 0
            ToolButton { action: alignLeftAction }
            ToolButton { action: alignCentralAction }
            ToolButton { action: alignRightAction }

            ToolButton { action: boldAction }
            ToolButton { action: italicAction }
            ToolButton { action: underlineAction }

            Item {
                width: 8
                anchors.top: parent.top
                anchors.bottom: parent.bottom
                anchors.margins: 6
                Rectangle {
                    width: 1
                    height: parent.height
                    anchors.horizontalCenter: parent.horizontalCenter
                    color: "#22000000"
                }
                Rectangle {
                    width: 1
                    height: parent.height
                    anchors.horizontalCenterOffset: 1
                    anchors.horizontalCenter: parent.horizontalCenter
                    color: "#33ffffff"
                }
            }

            //Цвет шрифта
            ToolButton
            {
                id: colorButton
                property var color : doc.textColor
                Rectangle
                {
                    id: colorRect
                    anchors.fill: parent
                    anchors.margins: 8
                    color: Qt.darker(doc.textColor,colorButton.pressed ? 1.4 : 1)
                    border.width: 1
                    border.color: Qt.darker(colorRect.color,2)
                }
                onClicked :
                {
                    colorDialog.color = doc.textColor
                    colorDialog.open()
                }
            }
            Item { Layout.fillWidth: true }
        }

    }

    ToolBar
    {
        id: fontInfoToolBar
        width: parent.width

        RowLayout
        {
            anchors.fill: parent

            //Бокс с текщим шрифтом
            ComboBox
            {
                id: fontFamilyComboBox
                implicitWidth: 150
                model: Qt.fontFamilies()
                property bool special: false
                onActivated:
                {
                    if(special == false||index!=0)
                    {
                        doc.fontFamily = textAt(index)
                    }
                }
            }

            //Размер Шрифта
            SpinBox
            {
                id: fontSizeSpinBox
                activeFocusOnPress: false
                implicitWidth: 50
                value: 0
                property bool valueGuard: true
                onValueChanged:
                {
                    if(valueGuard)
                        doc.fontSize = value
                }
            }

            Item
            {
                Layout.fillWidth: true
            }
        }
    }


    TextArea
    {
        id:textArea
        Accessible.name: "doc"
        frameVisible: false
        width: parent.width
        anchors.top: fontInfoToolBar.bottom
        anchors.bottom: parent.bottom
        baseUrl: "qrc:/"
        textFormat: Qt.RichText
        text: doc.text
        Component.onCompleted: forceActiveFocus()
    }

    MessageDialog
    {
        id: errorDialog
    }

    DocReader
    {
        id: doc
        target: textArea
        cursorPosition: textArea.cursorPosition
        selectionStart: textArea.selectionStart
        selectionEnd: textArea.selectionEnd
        textColor: colorDialog.color
        property string initPath: "qrc:/test/example.html"
        Component.onCompleted:
        {
            var xhr = new XMLHttpRequest;
            xhr.open("GET","tempFiles/initpath.txt")
            xhr.onreadystatechange = function()
            {
                if(xhr.readyState == XMLHttpRequest.DONE)
                    initPath = xhr.responseText;
            }

            doc.filePath = initPath
        }

        //Изменение размера шрифта
        onFontSizeChanged:
        {
            fontSizeSpinBox.valueGuard = false
            fontSizeSpinBox.value = doc.fontSize
            fontSizeSpinBox.valueGuard = true
        }

        //Изменение шрифта
        onFontFamilyChanged:
        {
            var index = Qt.fontFamilies().indexOf(doc.fontFamily)
            if(index == -1)
            {
                fontFamilyComboBox.currentIndex = 0
                fontFamilyComboBox.special = true
            }
            else
            {
                fontFamilyComboBox.currentIndex = index
                fontFamilyComboBox.special = false
            }
        }

        onError:
        {
            errorDialog.text = message
            errorDialog.visible = true
        }
    }
}
