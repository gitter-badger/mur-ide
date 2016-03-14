#include "processrunner.h"
#include <QDebug>

ProcessRunner::ProcessRunner(QObject *parent) :
    QObject(parent),
    m_process(new QProcess(this))
{
    QObject::connect(m_process, SIGNAL(readyReadStandardError()), SLOT(processOutReceived()));
    QObject::connect(m_process, SIGNAL(readyReadStandardOutput()), SLOT(processOutReceived()));
    QObject::connect(m_process, SIGNAL(readyRead()), SLOT(processOutReceived()));
    QObject::connect(m_process, SIGNAL(finished(int)), this, SIGNAL(finished(int)));
}

ProcessRunner::~ProcessRunner()
{
    if (m_process->isOpen()) {
        m_process->waitForFinished(500);
    }
    delete m_process;
}

QString ProcessRunner::path() const
{
    return m_path;
}

void ProcessRunner::setPath(QString path)
{
    m_path = path;
}

QStringList ProcessRunner::arguments() const
{
    return m_arguments;
}

void ProcessRunner::terminate()
{
    m_process->terminate();
}

void ProcessRunner::setArguments(QStringList arguments)
{
    m_arguments = arguments;
}

void ProcessRunner::run()
{
    if (m_path.isEmpty()) {
        emit finished(-1);
        return;
    }
    if (m_process->isOpen()) {
        m_process->waitForFinished();
    }

    m_process->start(m_path, m_arguments);
}


void ProcessRunner::processOutReceived()
{
    QString str = m_process->readAllStandardOutput().simplified();
    if (str != "") {
        emit processOutputReady(str);
    }
    str = m_process->readAllStandardError().simplified();
    if (str != "") {
        emit processOutputReady(str);
    }
    str = m_process->readAll().simplified();
    if (str != "") {
        emit processOutputReady(str);
    }
}

