#include "roboidetexteditor.h"
#include "logic/projectmanager.h"

#include <Qsci/qscilexercpp.h>
#include <QFontDatabase>
#include <QDebug>
#include <QFile>
#include <QFileDialog>
#include <QInputMethodEvent>
#include <QCoreApplication>
#include <QMessageBox>

RoboIdeTextEditor::RoboIdeTextEditor(QWidget *parent)
    : QsciScintilla(parent),
      m_isFileExist(false),
      m_pathToFile(QString(""))
{
    setupEditor();
    setupUi();
    QObject::connect(this, SIGNAL(textChanged()), this, SLOT(handleChangedText()));
}

void RoboIdeTextEditor::showContent(QString content, QString fileName)
{
    if (content.size()) {
        m_skipNullTextChanged = true;
    }
    setText(content);

    m_isFileExist = true;
    m_pathToFile = fileName;
}

void RoboIdeTextEditor::openFile(QString fileName)
{
    if (isModified()) {
        QMessageBox::StandardButton reply;
        reply = QMessageBox::question(this, "Сохранить?", "Данный файл не был сохранен. Сохнаить?",
                                    QMessageBox::Yes|QMessageBox::No);
        if (reply == QMessageBox::Yes) {
            saveFile();
        } else {
        }
    }

    QFile fileToOpen(fileName);
    if (!fileToOpen.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        qWarning() << "Something goes wrong at: RoboIdeTextEditor::openFile() [fileToOpen.open(...)]";
        return;
    }


    QString text;
    QTextStream in(&fileToOpen);
    in.setCodec("UTF-8");
    text = in.readAll();
    fileToOpen.close();
    if (text.size()) {
        m_skipNullTextChanged = true;
    }
    setText(text);
    setModified(false);
    m_isFileExist = true;
    m_pathToFile = fileName;
}

void RoboIdeTextEditor::saveFile()
{
    if (m_isFileExist)
    {
        if (ProjectManager::instance().getProject()->isOpened()) {
            emit projectFileSaved();
        }

        QFile fileToSave(m_pathToFile);
        if (!fileToSave.open(QIODevice::WriteOnly | QFile::Truncate | QIODevice::Text))
        {
            qWarning() << "Something goes wrong at: RoboIdeTextEditor::saveFile() [fileToOpen.open(...) / file exist]";
            return;
        }
        QTextStream out(&fileToSave);
        out.setCodec("UTF-8");
        QString writtenText = text();
        out << writtenText;
        fileToSave.close();
        setModified(false);
        return;
    }
    qDebug() << "Is exist: " << m_isFileExist;
    //! if file not exist
    saveFileAs();
}

void RoboIdeTextEditor::saveFileAs()
{
    QString fileName;
    fileName = QFileDialog::getSaveFileName(this, tr("Save File"),
            "",
            tr("Source (*.cpp)"));

    if (fileName.isEmpty())
    {
        qDebug() << "No file name specified at RoboIdeTextEditor::saveFileAs()";
        return;
    }

    QFile fileToSave(fileName);
    if (!fileToSave.open(QIODevice::WriteOnly | QIODevice::Text))
    {
        qWarning() << "Something goes wrong at: RoboIdeTextEditor::saveFileAs() [fileToOpen.open(...)]";
        return;
    }
    QTextStream out(&fileToSave);
    out.setCodec("UTF-8");
    out << text();
    fileToSave.close();
    setModified(false);
    m_isFileExist = true;
    m_pathToFile = fileName;
}

void RoboIdeTextEditor::blankFile()
{
    if (isModified()) {
        QMessageBox::StandardButton reply;
        reply = QMessageBox::question(this, "Сохранить?", "Данный файл не был сохранен. Сохнаить?",
                                    QMessageBox::Yes|QMessageBox::No);
        if (reply == QMessageBox::Yes) {
            saveFile();
        }
    }

    clear();
    m_pathToFile = "";
    m_isFileExist = false;
}

QString RoboIdeTextEditor::pathToFile() const
{
    return m_pathToFile;
}
void RoboIdeTextEditor::handleChangedText()
{
    if (ProjectManager::instance().getProject()->isOpened()) {
        if (m_skipNullTextChanged) {
            m_skipNullTextChanged = false;
        }
        else {
           emit textChanged(text());
        }
    }
}

void RoboIdeTextEditor::setupEditor()
{

    //! Setup encoding and font
    setUtf8(true);
    QFont font = QFont("Courier");

    font.setPointSize(10);

    //! Setup our C++ lexer
    m_lexCpp = new QsciLexerCPP(this);
    m_lexCpp->setFont(font);
    setupLexer();
    setLexer(m_lexCpp);

    //! Setup highlighting on current line
   setCaretLineVisible(true);
   setCaretLineBackgroundColor(QColor("#323232"));
   setCaretForegroundColor(QColor("#777777"));
   setMarginsForegroundColor(QColor("#777777"));

   //! Setup right margin
   setMarginsBackgroundColor(QColor("gainsboro"));
   setMarginLineNumbers(1, true);
   setMarginWidth(1, 30);

   //! Add some auto completion
   //setAutoCompletionSource(QsciScintilla::AcsAll);
   //setAutoCompletionCaseSensitivity(true);
   //setAutoCompletionUseSingle(QsciScintilla::AcusAlways);
   //setAutoCompletionThreshold(0);

   //! Setup indent for 5 spaces
   setAutoIndent(true);
   setIndentationGuides(false);
   setIndentationsUseTabs(true);
   setIndentationWidth(5);

   //! Brace matching highlighting
   setBraceMatching(QsciScintilla::SloppyBraceMatch);

   setMatchedBraceBackgroundColor("#76715E");
   setMatchedBraceForegroundColor("#FFFFFF");

   setUnmatchedBraceForegroundColor("#FFFFFF");
   setUnmatchedBraceBackgroundColor("#FA2772");

   //!Styles
   m_lexCpp->setPaper(QColor(37, 37, 37));
   setMarginsBackgroundColor(QColor(37, 37, 37));
   setStyleSheet("QWidget {"
                     "border-top: none;"
                     "border-left: none;"
                     "border-top: none;"
                     "border-right: none;"
                 "}");

}
//1c1c1c - MENU
//363636 - SELCET
void RoboIdeTextEditor::setupLexer()
{
    m_lexCpp->setColor("#FFFFFF", m_lexCpp->Default);
    m_lexCpp->setColor("#E7DB75", m_lexCpp->PreProcessor);
    m_lexCpp->setColor("#E7DB75", m_lexCpp->PreProcessorComment);

    m_lexCpp->setColor("#76715E", m_lexCpp->Comment);
    m_lexCpp->setColor("#76715E", m_lexCpp->CommentLine);
    m_lexCpp->setColor("#76715E", m_lexCpp->CommentDoc);
    m_lexCpp->setColor("#76715E", m_lexCpp->CommentDocKeyword);

    m_lexCpp->setColor("#66D9EE", m_lexCpp->Keyword);
    m_lexCpp->setColor("#AE82FF", m_lexCpp->KeywordSet2);

    m_lexCpp->setColor("#FE9720", m_lexCpp->SingleQuotedString);
    m_lexCpp->setColor("#FE9720", m_lexCpp->DoubleQuotedString);
    m_lexCpp->setColor("#FE9720", m_lexCpp->UnclosedString);

    m_lexCpp->setColor("#AE82FF", m_lexCpp->Number);
    m_lexCpp->setColor("#FFFFFF", m_lexCpp->Operator);

    m_lexCpp->setColor("#FFFFFF", m_lexCpp->Identifier);
    m_lexCpp->setColor("#36AF90", m_lexCpp->UserLiteral);
    m_lexCpp->setColor("#36AF90", m_lexCpp->UUID);
}

void RoboIdeTextEditor::setupUi()
{
    QFile editStyle(":/dark/styles/texteditscrollbar.css");
    editStyle.open(QFile::ReadOnly);
    QString styleSheet = editStyle.readAll();
    editStyle.close();

    setStyleSheet(styleSheet);
}

