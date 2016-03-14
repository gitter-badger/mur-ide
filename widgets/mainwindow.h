#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QtHelp>

#include "widgets/roboidetexteditor.h"
#include "widgets/roboideconsole.h"
#include "widgets/projecttree.h"
#include "widgets/helptreewidget.h"
#include "logic/sourcecompiler.h"
#include "logic/projectmanager.h"
#include "widgets/connecteddeviceslist.h"
#include "widgets/settingswidget.h"
#include "widgets/cameraswidget.h"
#include "filetransfer/wificonnection.h"

class MainWindow : public QMainWindow
{
    Q_OBJECT
public:
    MainWindow(QWidget *parent = 0);
    ~MainWindow();
private slots:
    void runCompilation();
    void compilationFinished();
    void uploadAndRun();
    void runApp();
    void killApp();
    void openFileOrProject();
    void openHelp();
    void switchCompilationTargetToEdison();
    void switchCompilationTargetToDesktop();
    void processOutReceived();

private:
    void createMenus();
    void createActions();
    void createToolBars();
    void createDockWindows();
    void connectActionsToSlots();

    QMenu *m_fileMenu;
    QMenu *m_editMenu;
    QMenu *m_viewMenu;
    QMenu *m_helpMenu;
    QMenu *m_compilationMenu;
    QMenu *m_settingsMenu;

    QToolBar *m_toolBar;

    QAction *m_newFileAct;
    QAction *m_saveAct;
    QAction *m_saveAsAct;
    QAction *m_undoAct;
    QAction *m_redoAct;
    QAction *m_copyAct;
    QAction *m_pasteAct;
    QAction *m_aboutAct;
    QAction *m_openFileAct;
    QAction *m_findAct;
    QAction *m_buildAct;
    QAction *m_uploadAndRunAct;
    QAction *m_showSettingsAct;
    QAction *m_runAppAct;
    QAction *m_stopAppAct;
    QAction *m_openHelpAct;
    QAction *m_edisonCompileAct;
    QAction *m_mingwCompileAct;

    RoboIdeTextEditor *m_roboIdeTextEdit;
    RoboIdeConsole *m_roboIdeConsole;
    SourceCompiler *m_sourceCompiller;
    ConnectedDevicesList *m_connectedDevicesList;
    SettingsWidget *m_settingsWidget;
    WiFiConnection *m_wifiConnection;
    QProcess *m_localApp;
};

#endif // MAINWINDOW_H
