#include "settingswidget.h"
#include "settingsmanager.h"

#include <QGridLayout>
#include <QPushButton>

SettingsWidget::SettingsWidget(QWidget *parent) :
    QDialog(parent),
    m_tabWidget(new QTabWidget(this)),
    m_wifiPasswordWidget(new WiFiPasswordWidget),
    m_compilerSettingsWidget(new CompilerSettingsWidget)
{
    //!Diable "?" button
    setWindowFlags(windowFlags() & ~Qt::WindowContextHelpButtonHint);

    QGridLayout *mainLayout = new QGridLayout(this);
    setLayout(mainLayout);

    mainLayout->addWidget(m_tabWidget);

    QPushButton *applyButton = new QPushButton("Применить", this);
    mainLayout->addWidget(applyButton);

    setupTabs();

    QString ip = SettingsManager::instance().ipAddress();
    QString login = SettingsManager::instance().userLogin();
    QString password = SettingsManager::instance().userPassword();

    QString pscpPath = SettingsManager::instance().pscpPath();
    QString plinkPath = SettingsManager::instance().plinkPath();

    m_wifiPasswordWidget->setTxInfo(pscpPath, plinkPath);
    m_wifiPasswordWidget->setLoginInfo(ip, login, password);

    QString sysroot = SettingsManager::instance().sysrootPath();
    QString ccOpt = SettingsManager::instance().compilerOptions();
    QString cc = SettingsManager::instance().compilerPath();
    QString make = SettingsManager::instance().mingwMakePath();

    m_compilerSettingsWidget->setCompilationSettings(cc, sysroot, ccOpt, make);

    QObject::connect(applyButton, SIGNAL(pressed()),this, SLOT(applySettings()));
}

void SettingsWidget::applySettings()
{
    SettingsManager::instance().setIpAddress(m_wifiPasswordWidget->ip());
    SettingsManager::instance().setUserLogin(m_wifiPasswordWidget->login());
    SettingsManager::instance().setUserPassword(m_wifiPasswordWidget->password());

    SettingsManager::instance().setSysrootPath(m_compilerSettingsWidget->sysrootPath());
    SettingsManager::instance().setCompilerOptions(m_compilerSettingsWidget->compilerOpt());
    SettingsManager::instance().setCompilerPath(m_compilerSettingsWidget->compilerPath());
    SettingsManager::instance().setMingwMakePath(m_compilerSettingsWidget->mingwMakePath());

    close();
}

void SettingsWidget::setupTabs()
{
    m_tabWidget->addTab(m_compilerSettingsWidget, "Компилятор");
    m_tabWidget->addTab(m_wifiPasswordWidget, "SSH");
}

