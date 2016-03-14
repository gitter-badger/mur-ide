#ifndef ROBOIDETEXTEDITOR_H
#define ROBOIDETEXTEDITOR_H

#include <QObject>
#include <QWidget>
#include <Qsci/qsciscintilla.h>
#include <Qsci/qsciapis.h>
#include <Qsci/qscilexercpp.h>

class RoboIdeTextEditor : public QsciScintilla
{
    Q_OBJECT
public:
    explicit RoboIdeTextEditor(QWidget *parent = 0);

signals:
    void textChanged(QString);
    void projectFileSaved();

public slots:
    void showContent(QString content, QString fileName);
    void openFile(QString fileName);
    void saveFile();
    void saveFileAs();
    void blankFile();
    QString pathToFile() const;
    void handleChangedText();

private:
    void setupEditor();
    void setupLexer();
    void setupUi();
    bool m_isFileExist;
    QString m_pathToFile;
    QsciLexerCPP* m_lexCpp;
    bool m_skipNullTextChanged = false;
};

#endif // ROBOIDETEXTEDITOR_H
