#ifndef CONNECTEDDEVICESLIST_H
#define CONNECTEDDEVICESLIST_H

#include <QWidget>
#include <QLabel>
#include <QIcon>

class ConnectedDevicesList : public QWidget
{
    Q_OBJECT
public:
    explicit ConnectedDevicesList(QWidget *parent = 0);

private:
    QLabel *createHeaderLabel(const QString &text);
    QLabel *createPixmapLabel();
public slots:
};

#endif // CONNECTEDDEVICESLIST_H
