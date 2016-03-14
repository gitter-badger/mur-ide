#ifndef WIFICONNECTION_H
#define WIFICONNECTION_H

#include "filetransfer/abstractconnection.h"
#include "logic/processrunner.h"
#include <QThread>

class WiFiConnection : public QObject
{
    Q_OBJECT
public:
    explicit WiFiConnection(QObject *parent = 0);
    ~WiFiConnection();
    void prepare();
    int sendAndRun(QString file);
    QStringList appList();
    void runApp();
    void killApp();
    void stopAppByName(QString bin);
    void send(QString file);
signals:
    void run();
    void onExecOutput(QString);

public slots:
private slots:
    void onExecFinished(int retCode);
private:
    QString m_binaryPath;

    void* m_zmqContext;
    void* m_zmqReqSoc;
};

#endif // WIFICONNECTION_H
