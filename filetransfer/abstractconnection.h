#ifndef ABSTRACTCONNECTION_H
#define ABSTRACTCONNECTION_H

#include <QObject>

class AbstractConnection : public QObject
{
    Q_OBJECT
public:
    explicit AbstractConnection(QObject *parent = 0);
    virtual ~AbstractConnection();
    virtual void prepare() = 0;
    virtual int sendAndRun(QString) = 0;
    virtual QStringList appList() = 0;
    virtual void runApp() = 0;
    virtual void stopAppByName(QString) = 0;
    virtual void send(QString) = 0;
    bool isReady() { return m_isReady; }
signals:
    void prepareFinished(int);
protected:
    bool m_isReady = true;
signals:

public slots:
};

#endif // ABSTRACTCONNECTION_H
