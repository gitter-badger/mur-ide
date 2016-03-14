#include "roboideconsole.h"

#include <QPalette>
#include <QTextCursor>
#include <QDebug>
#include <iostream>

RoboIdeConsole::RoboIdeConsole(QWidget *parent) : QTextEdit(parent)
{
    QPalette pal = palette();
    pal.setColor(QPalette::Base, QColor("#252525"));
    pal.setColor(QPalette::Text, "#CCCCCC");
    setPalette(pal);
    setReadOnly(true);
    //! We have to use setStyleSheet instead of setFontFamily().
    //! If we'll use setFontFamily() QTextEdit font settings will be reset to default after deleting all data from widget.
    setStyleSheet("QTextEdit {"
                  "font-family: Courier;"
                  "font-style: normal;"
                  "font-size: 12pt;"
                  "};");
}


