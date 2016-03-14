#ifndef COMPILERSETTINGSWIDGET_H
#define COMPILERSETTINGSWIDGET_H

#include <QWidget>
#include <QLineEdit>

class CompilerSettingsWidget : public QWidget
{
    Q_OBJECT
public:
    explicit CompilerSettingsWidget(QWidget *parent = 0);
    void setCompilationSettings(const QString &ccPath, const QString &sysrootPath, const QString &ccOpt, const QString &makePath);
    QString sysrootPath();
    QString compilerOpt();
    QString compilerPath();
    QString mingwMakePath();
signals:

public slots:
private:
    QLineEdit *m_sysrootEdit;
    QLineEdit *m_compilerPathEdit;
    QLineEdit *m_compilerOptEdit;
    QLineEdit *m_mingwMakePathEdit;
    bool m_isValid = false;
};

#endif // COMPILERSETTINGSWIDGET_H
