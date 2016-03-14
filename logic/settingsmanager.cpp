#include "settingsmanager.h"
#include <QDebug>
#include <QCoreApplication>

SettingsManager &SettingsManager::instance()
{
    static SettingsManager instance;
    return instance;
}

SettingsManager::~SettingsManager()
{
    delete m_settings;
}

QByteArray SettingsManager::mainWindowState() const
{
    m_settings->beginGroup("MainWindowState");
    QByteArray state = m_settings->value("State", 0).toByteArray();
    m_settings->endGroup();
    return state;
}

void SettingsManager::setMainWindowState(const QByteArray &state)
{
    m_settings->beginGroup("MainWindowState");
    m_settings->setValue("State", state);
    m_settings->endGroup();
}

QString SettingsManager::plinkPath() const
{
    m_settings->beginGroup("wi-fi-connection-settings");
    QString plink = m_settings->value("Plink",
                                        QCoreApplication::applicationDirPath() + "\\tools\\plink.exe").toString();

    m_settings->endGroup();
    return plink;
}

void SettingsManager::setPlinkPath(const QString &path)
{
    m_settings->beginGroup("wi-fi-connection-settings");
    m_settings->setValue("Plink", path);
    m_settings->endGroup();
}

QString SettingsManager::pscpPath() const
{
    m_settings->beginGroup("wi-fi-connection-settings");
    QString pscp = m_settings->value("Pscp",
                                        QCoreApplication::applicationDirPath() + "\\tools\\pscp.exe").toString();
    m_settings->endGroup();
    return pscp;
}

void SettingsManager::setPscpPath(const QString &path)
{
    m_settings->beginGroup("wi-fi-connection-settings");
    m_settings->setValue("Pscp", path);
    m_settings->endGroup();
}

QString SettingsManager::sysrootPath() const
{
    m_settings->beginGroup("i586-poky-linux");
    QString sysroot = m_settings->value("Sysroot",
                                        QCoreApplication::applicationDirPath() + "\\devkit-x86\\sysroots\\i586-poky-linux").toString();
    m_settings->endGroup();
    return sysroot;
}

void SettingsManager::setSysrootPath(const QString &path)
{
    m_settings->beginGroup("i586-poky-linux");
    m_settings->setValue("Sysroot", path);
    m_settings->endGroup();
}

QString SettingsManager::compilerPath() const
{
    m_settings->beginGroup("i586-poky-linux");
    QString compiler = m_settings->value("CC",
                                        QCoreApplication::applicationDirPath() +"\\devkit-x86\\sysroots\\x86_64-pokysdk-mingw32\\usr\\bin\\i586-poky-linux\\i586-poky-linux-g++.exe").toString();
    m_settings->endGroup();
    return compiler;
}

void SettingsManager::setCompilerPath(const QString &path)
{
    m_settings->beginGroup("i586-poky-linux");
    m_settings->setValue("CC", path);
    m_settings->endGroup();
}

QString SettingsManager::mingwMakePath() const
{
    m_settings->beginGroup("i586-poky-linux");
    QString make = m_settings->value("mingwMake", QCoreApplication::applicationDirPath() + "\\tools\\mingw32-make.exe").toString();
    m_settings->endGroup();
    return make;
}

void SettingsManager::setMingwMakePath(const QString &path)
{
    m_settings->beginGroup("i586-poky-linux");
    m_settings->setValue("mingwMake", path);
    m_settings->endGroup();
}

QString SettingsManager::ipAddress() const
{
    m_settings->beginGroup("IpLoginPassword");
    QString ip = m_settings->value("IP", "192.168.42.1").toString();
    m_settings->endGroup();
    return ip;
}

void SettingsManager::setIpAddress(const QString &ip)
{
    m_settings->beginGroup("IpLoginPassword");
    m_settings->setValue("IP", ip);
    m_settings->endGroup();
}

QString SettingsManager::userLogin() const
{
    m_settings->beginGroup("IpLoginPassword");
    QString login = m_settings->value("Login", "root").toString();
    m_settings->endGroup();
    return login;
}

void SettingsManager::setUserLogin(const QString &login)
{
    m_settings->beginGroup("IpLoginPassword");
    m_settings->setValue("Login", login);
    m_settings->endGroup();
}

QString SettingsManager::userPassword() const
{
    m_settings->beginGroup("IpLoginPassword");
    QString password = m_settings->value("Password", "2030405060").toString();
    m_settings->endGroup();
    return password;
}

void SettingsManager::setUserPassword(const QString &password) const
{
    m_settings->beginGroup("IpLoginPassword");
    m_settings->setValue("Password", password);
    m_settings->endGroup();
}

QString SettingsManager::compilerOptions() const
{
    m_settings->beginGroup("i586-poky-linux");
    QString compilerOpt = m_settings->value("CCOPT", " -O2 -lstdc++ ").toString();
    m_settings->endGroup();
    return compilerOpt;
}

void SettingsManager::setCompilerOptions(const QString &options) const
{
    m_settings->beginGroup("i586-poky-linux");
    m_settings->setValue("CCOPT", options);
    m_settings->endGroup();
}

SettingsManager::SettingsManager(QObject *parent) :
    QObject(parent),
    m_settings(new QSettings("roboide_settings.ini", QSettings::IniFormat))

{

}

