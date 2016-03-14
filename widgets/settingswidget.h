#ifndef SETTINGSWIDGET_H
#define SETTINGSWIDGET_H

#include <QDialog>
#include <QTabWidget>

#include "wifipasswordwidget.h"
#include "compilersettingswidget.h"
class SettingsWidget : public QDialog
{
    Q_OBJECT
public:
    explicit SettingsWidget(QWidget *parent = 0);

signals:

public slots:
private slots:
    void applySettings();
private:
    void setupTabs();
    QTabWidget *m_tabWidget;
    WiFiPasswordWidget *m_wifiPasswordWidget;
    CompilerSettingsWidget *m_compilerSettingsWidget;

};

#endif // SETTINGSWIDGET_H
