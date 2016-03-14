#-------------------------------------------------
#
# Project created by QtCreator 2015-10-02T14:39:32
#
#-------------------------------------------------

QT       += core gui network help

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = RoboIDE
TEMPLATE = app

CONFIG += c++11

win32 {

defineTest(copyToDestdir) {
    files = $$1
    for(FILE, files) {
        DDIR = $$OUT_PWD
        # Replace slashes in paths with backslashes for Windows
        win32:FILE ~= s,/,\\,g
        win32:DDIR ~= s,/,\\,g
        QMAKE_POST_LINK += $$QMAKE_COPY $$quote($$FILE) $$quote($$DDIR) $$escape_expand(\\n\\t)
    }
    export(QMAKE_POST_LINK)
}

INCLUDEPATH += "$$PWD\extlibs\qsci\include"
DEPENDPATH += "$$PWD\extlibs\qsci\include"

INCLUDEPATH += "$$PWD\extlibs\zmq\include"
INCLUDEPATH += "$$PWD\extlibs\zmq\include"

CONFIG(release, debug | release) : {
    message(Building for Windows release mode)
    LIBS += -L"$$PWD\extlibs\qsci\mingw32\lib\release"
    LIBS += -L"$$PWD\extlibs\zmq\mingw32\lib"
    LIBS += -lqscintilla2 -llibzmq

    EXTRA_DLL += \
        $$PWD\extlibs\qsci\mingw32\bin\release\qscintilla2.dll \
        $$PWD\extlibs\zmq\mingw32\bin\libzmq.dll
}

CONFIG(debug, debug | release) : {
    message(Building for Windows debug mode)
    LIBS += -L"$$PWD\extlibs\qsci\mingw32\lib\debug"
    LIBS += -L"$$PWD\extlibs\zmq\mingw32\lib"
    LIBS +=  -lqscintilla2 -llibzmq

    EXTRA_DLL += \
        $$PWD\extlibs\qsci\mingw32\bin\debug\qscintilla2.dll \
        $$PWD\extlibs\zmq\mingw32\bin\libzmq.dll
}

# Replace slashes in paths with backslashes for Windows
#EXTRA_DLL ~= s,/,\\,g

copyToDestdir($$EXTRA_DLL)

QMAKE_EXTRA_TARGETS += copyfiles
POST_TARGETDEPS += copyfiles

}

Linux {
    LIBS += -lqscintilla2 -lzmq
}

include(widgets/widgets.pri)
include(logic/logic.pri)
include(filetransfer/filetransfer.pri)

SOURCES += main.cpp

HEADERS  +=

RESOURCES += \
    icons.qrc \
    styles.qrc

DISTFILES +=
