#ifndef WIFIPASSWORDWIDGET_H
#define WIFIPASSWORDWIDGET_H

#include <QWidget>
#include <QPair>
#include <QString>
#include <QLineEdit>
#include <QDialog>

class WiFiPasswordWidget : public QWidget
{
    Q_OBJECT
public:
    void setLoginInfo(const QString &ip, const QString &login, const QString password);
    void setTxInfo(const QString &pscp, const QString &plink);
    explicit WiFiPasswordWidget(QWidget *parent = 0);
    QString login();
    QString password();
    QString ip();
    QString pscp();
    QString plink();

signals:
    void okayButtonPressed();
public slots:
private:
    QLineEdit *m_passwordEdit;
    QLineEdit *m_loginEdit;
    QLineEdit *m_ipEdit;
    QLineEdit *m_pscpEdit;
    QLineEdit *m_plinkEdit;
    bool m_isValid = false;
};

#endif // WIFIPASSWORDWIDGET_H
