#include "wifipasswordwidget.h"

#include <QVariant>
#include <QGridLayout>
#include <QLabel>
#include <QRegExp>
#include <QMessageBox>
#include <QPushButton>
#include <QHostAddress>
#include <QSpacerItem>

WiFiPasswordWidget::WiFiPasswordWidget(QWidget *parent) :
    QWidget(parent),
    m_passwordEdit(new QLineEdit),
    m_loginEdit(new QLineEdit),
    m_ipEdit(new QLineEdit),
    m_pscpEdit(new QLineEdit),
    m_plinkEdit(new QLineEdit)
{
    m_passwordEdit->setEchoMode(QLineEdit::Password);

    QGridLayout *layout = new QGridLayout(this);
    setLayout(layout);

    QLabel *ipLabel = new QLabel("IP: ", this);
    QLabel *loginLabel = new QLabel("Логин: ", this);
    QLabel *passwordLabel = new QLabel("Пароль: ", this);
    QLabel *pscpLabel = new QLabel("Путь PSCP:", this);
    QLabel *plinkPath = new QLabel("Путь до Plink:", this);

    layout->addWidget(ipLabel, 0, 0);
    layout->addWidget(loginLabel, 1, 0);
    layout->addWidget(passwordLabel, 2, 0);
    layout->addWidget(pscpLabel,3, 0);
    layout->addWidget(plinkPath, 4, 0);

    layout->addWidget(m_ipEdit, 0, 1);
    layout->addWidget(m_loginEdit, 1, 1);
    layout->addWidget(m_passwordEdit, 2, 1);
    layout->addWidget(m_pscpEdit, 3, 1);
    layout->addWidget(m_plinkEdit, 4, 1);
}

void WiFiPasswordWidget::setLoginInfo(const QString &ip, const QString &login, const QString password)
{
    m_loginEdit->setText(login);
    m_passwordEdit->setText(password);
    m_ipEdit->setText(ip);
}

void WiFiPasswordWidget::setTxInfo(const QString &pscp, const QString &plink)
{
   m_plinkEdit->setText(plink);
   m_pscpEdit->setText(pscp);
}


QString WiFiPasswordWidget::login()
{
    return m_loginEdit->text();
}

QString WiFiPasswordWidget::password()
{
    return m_passwordEdit->text();
}

QString WiFiPasswordWidget::ip()
{
    return m_ipEdit->text();
}

QString WiFiPasswordWidget::pscp()
{
    return m_pscpEdit->text();
}

QString WiFiPasswordWidget::plink()
{
    return m_plinkEdit->text();
}
