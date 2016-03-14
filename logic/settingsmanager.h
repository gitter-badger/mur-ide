#ifndef SETTINGSMANAGER_H
#define SETTINGSMANAGER_H

#include <QObject>
#include <QSettings>
#include <QPoint>
#include <QSize>
#include <QByteArray>

class SettingsManager : public QObject
{
    Q_OBJECT
public:
    static SettingsManager& instance();
    ~SettingsManager();

    QByteArray mainWindowState() const;
    void setMainWindowState(const QByteArray &state);

    QString plinkPath() const;
    void setPlinkPath(const QString &path);

    QString pscpPath() const;
    void setPscpPath(const QString &path);

    QString sysrootPath() const;
    void setSysrootPath(const QString &path);

    QString compilerPath() const;
    void setCompilerPath(const QString &path);

    QString mingwMakePath() const;
    void setMingwMakePath(const QString &path);

    QString ipAddress() const;
    void setIpAddress(const QString &ip);

    QString userLogin() const;
    void setUserLogin(const QString &login);

    QString userPassword() const;
    void setUserPassword(const QString &password) const;

    QString compilerOptions() const;
    void setCompilerOptions(const QString &options) const;
signals:

public slots:
private:
    explicit SettingsManager(QObject *parent = 0);
    SettingsManager(SettingsManager const&) = delete;
    void operator = (SettingsManager const&) = delete;
    QSettings *m_settings;
};

#endif // SETTINGSMANAGER_H
