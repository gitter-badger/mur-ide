#ifndef SOURCECOMPILER_H
#define SOURCECOMPILER_H

#include <QObject>
#include <QProcess>
#include <QThread>
#include <QSettings>

#include "logic/processrunner.h"

class SourceCompiler : public QObject
{
    Q_OBJECT
public:
    enum  class TARGET : int {
        MINGW = 10,
        EDISON = 20
    };
    explicit SourceCompiler(QObject *parent = 0);
    virtual ~SourceCompiler();
    QString pathToBinary() const;
    bool isCompiled() const;
    void setTarget(TARGET target);
signals:
    void onCompilationOutput(QString output);
    void finished();
    void run();
public slots:
    void onRunCompilation(QString src);
private slots:
    void onCompilationFinished(int retCode);
private:
    QThread *m_compilationThread;
    ProcessRunner *m_processRunner;
    QString m_pathToBinary;
    bool m_isCompiled;
    TARGET m_target;
};

#endif // SOURCECOMPILER_H
