#ifndef ROBOIDECONSOLE_H
#define ROBOIDECONSOLE_H

#include <QWidget>
#include <QPlainTextEdit>

class RoboIdeConsole : public QTextEdit
{
    Q_OBJECT
public:
    explicit RoboIdeConsole(QWidget *parent = 0);

signals:

public slots:
};

#endif // ROBOIDECONSOLE_H
