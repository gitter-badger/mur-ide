#include "sourcecompiler.h"
#include "settingsmanager.h"
#include "projectmanager.h"

#include <QDebug>
#include <QFileInfo>
#include <QRunnable>
#include <QtConcurrent/QtConcurrent>
#include <QMessageBox>
#include <QDateTime>
#include <thread>
#include <chrono>

SourceCompiler::SourceCompiler(QObject *parent) :
    QObject(parent),
    m_compilationThread(new QThread(this)),
    m_processRunner(new ProcessRunner),
    m_isCompiled(false),
    m_target(TARGET::EDISON)
{
    m_processRunner->moveToThread(m_compilationThread);
    m_compilationThread->start();
    QObject::connect(m_processRunner, SIGNAL(processOutputReady(QString)), this, SIGNAL(onCompilationOutput(QString)));
    QObject::connect(m_processRunner, SIGNAL(finished(int)), this, SLOT(onCompilationFinished(int)));
    QObject::connect(this, SIGNAL(run()), m_processRunner, SLOT(run()));
}

SourceCompiler::~SourceCompiler()
{
    m_compilationThread->terminate();
}

QString SourceCompiler::pathToBinary() const
{
    return m_pathToBinary;
}

bool SourceCompiler::isCompiled() const
{
    return m_isCompiled;
}

void SourceCompiler::setTarget(SourceCompiler::TARGET target)
{
    m_target = target;
}

void SourceCompiler::onRunCompilation(QString src)
{   
    if (m_target == TARGET::EDISON) {
        QString pathToSysRoot = SettingsManager::instance().sysrootPath();
        QString pathToCC = SettingsManager::instance().compilerPath();
        QString options = SettingsManager::instance().compilerOptions();

        QString processPath;
        QStringList processArgs;

        QString binaryName = src + ".bin ";

        processPath = pathToCC;
        processArgs << "--sysroot=" + pathToSysRoot;
        processArgs << src;
        processArgs << "-o" + binaryName;
        processArgs << options.split(" ", QString::SkipEmptyParts);

        m_pathToBinary = src + ".bin";
        qDebug() << processArgs;
        m_processRunner->setArguments(processArgs);
        m_processRunner->setPath(processPath);
    }

    if (m_target == TARGET::MINGW) {
        QString pathToIncludes = "-ID:\\Projects\\RoboIDE\\build-RoboIDE-Desktop_Qt_5_5_1_MinGW_32bit-Debug\\debug\\mingw492_32\\murlibs";
        QString pathToLibs = "-LD:\\Projects\\RoboIDE\\build-RoboIDE-Desktop_Qt_5_5_1_MinGW_32bit-Debug\\debug\\mingw492_32\\murlibs";

        QString pathToCC = "D:\\Projects\\RoboIDE\\build-RoboIDE-Desktop_Qt_5_5_1_MinGW_32bit-Debug\\debug\\mingw492_32\\bin\\g++.exe";
        QString options = "-lopencv_core2412 -lopencv_highgui2412 -std=c++11";

        QString processPath;
        QStringList processArgs;

        QString binaryName = src + ".bin";

        processPath = pathToCC;
        processArgs << pathToIncludes;
        processArgs << pathToLibs;
        processArgs << src;
        processArgs << "-o" + binaryName;
        processArgs << options.split(" ", QString::SkipEmptyParts);

        m_pathToBinary = src + ".bin";
        qDebug() << processArgs;
        m_processRunner->setArguments(processArgs);
        m_processRunner->setPath(processPath);
    }

    emit run();
}

void SourceCompiler::onCompilationFinished(int retCode)
{
    qDebug() << retCode;
    if (retCode != 0) {
        m_pathToBinary = "";
        m_isCompiled = false;
        onCompilationOutput(QString("Ошибка компиляции " + QDateTime::currentDateTime().toString()));
    }
    m_isCompiled = true;
    if (retCode == 0) {
        onCompilationOutput(QString("Компиляция прошла успешно " + QDateTime::currentDateTime().toString()));
    }
    emit finished();
}


