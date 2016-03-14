#include "mainwindow.h"
#include "roboideconsole.h"
#include "wifipasswordwidget.h"
#include "settingsmanager.h"
#include "projectmanager.h"

#include <QToolBar>
#include <QAction>
#include <QMenu>
#include <QMenuBar>
#include <QClipboard>
#include <QDockWidget>
#include <QFileDialog>
#include <QDebug>
#include <QJsonDocument>
#include <QCoreApplication>
#include <QDesktopServices>
#include <QMessageBox>
#include <QtHelp>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent),
      m_roboIdeTextEdit(new RoboIdeTextEditor(this)),
      m_roboIdeConsole(new RoboIdeConsole(this)),
      m_sourceCompiller(new SourceCompiler(this)),
      m_connectedDevicesList(new ConnectedDevicesList(this)),
      m_settingsWidget(new SettingsWidget(this)),
      m_wifiConnection(new WiFiConnection(this)),
      m_localApp(new QProcess(this))
{
    setCentralWidget(m_roboIdeTextEdit);
    createActions();
    createMenus();
    createToolBars();
    createDockWindows();
    connectActionsToSlots();
    showMaximized();

    QByteArray state = SettingsManager::instance().mainWindowState();
    if (state.size() != 0) {
        restoreState(state);
    }

    setStyleSheet("QMainWindow::separator \
    { \
        background-color: #464646;\
        width: 1px;\
        border: none;\
    }");
    m_connectedDevicesList->setVisible(false);
}

MainWindow::~MainWindow()
{
    QByteArray state = saveState();
    SettingsManager::instance().setMainWindowState(state);
}

void MainWindow::runCompilation()
{
    m_buildAct->setEnabled(false);
    if (m_roboIdeTextEdit->isModified()) {
        QMessageBox::StandardButton reply;
        reply = QMessageBox::question(this, "Сохранить?", "Данный файл не был сохранен. Сохнаить?",
                                    QMessageBox::Yes|QMessageBox::No);
        if (reply == QMessageBox::Yes) {
            m_roboIdeTextEdit->saveFile();
        }
    }
    if (m_edisonCompileAct->isChecked())
    {
        m_sourceCompiller->setTarget(SourceCompiler::TARGET::EDISON);
    }
    if (m_mingwCompileAct->isChecked())
    {
        m_sourceCompiller->setTarget(SourceCompiler::TARGET::MINGW);
    }
    m_sourceCompiller->onRunCompilation(m_roboIdeTextEdit->pathToFile());
}

void MainWindow::compilationFinished()
{
    qDebug() << "Compilation finished";
    m_buildAct->setEnabled(true);
}

void MainWindow::uploadAndRun()
{
    m_uploadAndRunAct->setDisabled(true);
    m_wifiConnection->sendAndRun(m_sourceCompiller->pathToBinary());
    m_uploadAndRunAct->setEnabled(true);
    m_roboIdeConsole->append("Программа отправлена!");
}

void MainWindow::runApp()
{
    if (m_edisonCompileAct->isChecked()) {
        m_runAppAct->setDisabled(true);
        m_wifiConnection->runApp();
        m_runAppAct->setEnabled(true);
    }
    if (m_mingwCompileAct->isChecked()) {
        if (m_localApp->isOpen()) {
            m_localApp->kill();
        }
        QProcessEnvironment env = QProcessEnvironment::systemEnvironment();
        env.insert("PATH", env.value("Path") + ";D:\\Projects\\RoboIDE\\build-RoboIDE-Desktop_Qt_5_5_1_MinGW_32bit-Debug\\debug\\mingw492_32\\murlibs");
        m_localApp->setProcessEnvironment(env);
        m_localApp->start(m_sourceCompiller->pathToBinary());
        qDebug() << m_localApp->environment();

    }
    m_roboIdeConsole->append("Программа запущена!");
}

void MainWindow::killApp()
{
    if (m_edisonCompileAct->isChecked()) {
        m_stopAppAct->setDisabled(true);
        m_wifiConnection->killApp();
        m_stopAppAct->setEnabled(true);
    }
    if (m_mingwCompileAct->isChecked()) {
        m_localApp->kill();
    }
    m_roboIdeConsole->append("Программа остановлена!");


}

void MainWindow::openFileOrProject()
{
    QFileDialog dialog(this);
    dialog.setNameFilter("Robo files (*.h *.cpp)");
    QStringList fileNames;
    if (dialog.exec()) {
        fileNames = dialog.selectedFiles();
    }
    else {
        qWarning() << "Something goes wrong at: MainWindow::openFileOrProject() [dialog.exec()]";
        return;
    }

    if (fileNames.isEmpty()) {
        return;
    }

    QString fileName = fileNames.at(0);
    m_roboIdeTextEdit->openFile(fileName);
}

void MainWindow::openHelp()
{
    QString file =  "file:///" + QCoreApplication::applicationDirPath() + "/help.pdf";
    QDesktopServices::openUrl(QUrl(file));
}

void MainWindow::switchCompilationTargetToEdison()
{
    m_mingwCompileAct->setChecked(false);
}

void MainWindow::switchCompilationTargetToDesktop()
{
    m_edisonCompileAct->setChecked(false);
}

void MainWindow::processOutReceived()
{
    QString str = m_localApp->readAllStandardOutput().simplified();
    if (str != "") {
        m_roboIdeConsole->append(str);
    }
    str = m_localApp->readAllStandardError().simplified();
    if (str != "") {
        m_roboIdeConsole->append(str);
    }
    str = m_localApp->readAll().simplified();
    if (str != "") {
        m_roboIdeConsole->append(str);
    }
}



void MainWindow::createActions()
{
    //!Edit file actions

    m_redoAct = new QAction(QIcon(":/icons/icons/tools/redo.png"), tr("Повторить"), this);
    m_redoAct->setShortcut(QKeySequence::Redo);
    m_redoAct->setIconVisibleInMenu(false);

    m_undoAct = new QAction(QIcon(":/icons/icons/tools/undo.png"), tr("Отменить"), this);
    m_undoAct->setShortcut(QKeySequence::Undo);
    m_undoAct->setIconVisibleInMenu(false);

    m_saveAct = new QAction(QIcon(":/icons/icons/tools/save.png"), tr("Сохранить"), this);
    m_saveAct->setShortcut(QKeySequence::Save);
    m_saveAct->setIconVisibleInMenu(false);

    m_saveAsAct = new QAction(QIcon(":/icons/icons/tools/save-as.png"), tr("Сохранить как..."), this);
    m_saveAsAct->setShortcut(QKeySequence::SaveAs);
    m_saveAsAct->setIconVisibleInMenu(false);

    m_newFileAct = new QAction(QIcon(":/icons/icons/tools/new-file.png"), tr("Новый файл"), this);
    m_newFileAct->setShortcut(QKeySequence::New);
    m_newFileAct->setIconVisibleInMenu(false);

    m_pasteAct = new QAction(QIcon(":/icons/icons/tools/paste.png"), tr("Вставить"), this);
    m_pasteAct->setShortcut(QKeySequence::Paste);
    m_pasteAct->setIconVisibleInMenu(false);

    m_copyAct = new QAction(QIcon(":/icons/icons/tools/copy.png"), tr("Копировать"), this);
    m_copyAct->setShortcut(QKeySequence::Copy);
    m_copyAct->setIconVisibleInMenu(false);

    m_openFileAct = new QAction(QIcon(":/icons/icons/tools/open.png"), tr("Открыть файл"), this);
    m_openFileAct->setShortcut(QKeySequence::Open);
    m_openFileAct->setIconVisibleInMenu(false);

    m_findAct = new QAction(QIcon(":/icons/icons/tools/search.png"), tr("Поиск"), this);
    m_findAct->setShortcut(QKeySequence::Find);
    m_findAct->setIconVisibleInMenu(false);

    //!Compilation actions
    m_buildAct = new QAction(QIcon(":/icons/icons/tools/compile.png"), tr("Проверить"), this);
    m_buildAct->setShortcut(QKeySequence(Qt::CTRL + Qt::Key_B));
    m_buildAct->setIconVisibleInMenu(false);

    m_uploadAndRunAct = new QAction(QIcon(":/icons/icons/tools/upload.png"), tr("Отправить программу"), this);
    m_uploadAndRunAct->setShortcut(QKeySequence(Qt::CTRL + Qt::Key_R));
    m_uploadAndRunAct->setIconVisibleInMenu(false);

    m_stopAppAct = new QAction(QIcon(":/icons/icons/tools/stop.png"), tr("Остановаить программу"), this);
    m_stopAppAct->setShortcut(QKeySequence(Qt::CTRL + Qt::SHIFT + Qt::Key_S));
    m_stopAppAct->setIconVisibleInMenu(false);

    m_runAppAct = new QAction(QIcon(":/icons/icons/tools/start.png"), tr("Запустить программу"), this);
    m_runAppAct->setShortcut(QKeySequence(Qt::CTRL + Qt::SHIFT + Qt::Key_R));
    m_runAppAct->setIconVisibleInMenu(false);

    m_showSettingsAct = new QAction(QIcon(":/icons/actions/preferences-desktop.png"), tr("Настройки"), this);
    m_showSettingsAct->setIconVisibleInMenu(false);

    m_openHelpAct = new QAction(tr("Справка"), this);
    m_openHelpAct->setShortcut(QKeySequence(Qt::Key_F1));

    m_edisonCompileAct = new QAction(tr("MUR Robot"), this);
    m_edisonCompileAct->setCheckable(true);
    m_edisonCompileAct->setChecked(true);
    m_mingwCompileAct = new QAction(tr("Desktop"), this);
    m_mingwCompileAct->setCheckable(true);
}

void MainWindow::createToolBars()
{

    QFile toolBar(":/dark/styles/toolbar.css");
    toolBar.open(QFile::ReadOnly);
    QString styleSheet = toolBar.readAll();
    toolBar.close();

    m_toolBar = addToolBar(tr("Панель устройств"));
    m_toolBar->setStyleSheet(styleSheet);
    m_toolBar->setMovable(false);
    m_toolBar->setObjectName("ToolBar");
    m_viewMenu->addAction(m_toolBar->toggleViewAction());
    m_toolBar->setIconSize(QSize(22, 22));

    m_toolBar->addAction(m_newFileAct);
    m_toolBar->addAction(m_openFileAct);
    m_toolBar->addAction(m_saveAct);
    m_toolBar->addAction(m_saveAsAct);
    m_toolBar->addSeparator();

    m_toolBar->addAction(m_copyAct);
    m_toolBar->addAction(m_pasteAct);
    m_toolBar->addAction(m_findAct);
    m_toolBar->addAction(m_undoAct);
    m_toolBar->addAction(m_redoAct);
    m_toolBar->addSeparator();

    m_toolBar->addAction(m_buildAct);
    m_toolBar->addAction(m_uploadAndRunAct);
    m_toolBar->addAction(m_runAppAct);
    m_toolBar->addAction(m_stopAppAct);
}

void MainWindow::createMenus()
{
    QFile fileMenuStyle(":/dark/styles/filemenu.css");
    fileMenuStyle.open(QFile::ReadOnly);
    QString styleSheet = fileMenuStyle.readAll();
    fileMenuStyle.close();
    m_fileMenu = menuBar()->addMenu(tr("&Файл"));
    menuBar()->setStyleSheet(styleSheet);

    m_fileMenu->addAction(m_newFileAct);
    m_fileMenu->addAction(m_openFileAct);
    m_fileMenu->addAction(m_saveAct);
    m_fileMenu->addAction(m_saveAsAct);

    m_editMenu = menuBar()->addMenu(tr("&Правка"));
    m_editMenu->addAction(m_redoAct);
    m_editMenu->addAction(m_undoAct);
    m_editMenu->addAction(m_copyAct);
    m_editMenu->addAction(m_pasteAct);
    m_editMenu->addAction(m_findAct);

    m_compilationMenu = menuBar()->addMenu(tr("&Компиляция"));
    m_compilationMenu->addAction(m_buildAct);
    m_compilationMenu->addAction(m_uploadAndRunAct);
    m_compilationMenu->addAction(m_edisonCompileAct);
    m_compilationMenu->addAction(m_mingwCompileAct);

    m_settingsMenu = menuBar()->addMenu(tr("&Настройки"));
    m_settingsMenu->addAction(m_showSettingsAct);

    m_viewMenu = menuBar()->addMenu(tr("Окно"));

    m_helpMenu = menuBar()->addMenu(tr("Справка"));
    m_helpMenu->addAction(m_openHelpAct);
}

void MainWindow::createDockWindows()
{
    QFile dockSheet(":/dark/styles/dock.css");
    dockSheet.open(QFile::ReadOnly);
    QString styleSheet = dockSheet.readAll();
    dockSheet.close();

    QDockWidget *dock = new QDockWidget(tr("Консоль"), this);
    dock->setObjectName("ConsoleDockWidget");
    dock->setAllowedAreas(Qt::LeftDockWidgetArea | Qt::RightDockWidgetArea
                          | Qt::BottomDockWidgetArea | Qt::TopDockWidgetArea);
    dock->setWidget(m_roboIdeConsole);
    addDockWidget(Qt::BottomDockWidgetArea, dock);
    m_viewMenu->addAction(dock->toggleViewAction());
    dock->setStyleSheet(styleSheet);

    dock = new QDockWidget(tr("Устройства"), this);
    dock->setObjectName("ConnectedDevicesListDockWidget");
    dock->setAllowedAreas(Qt::LeftDockWidgetArea | Qt::RightDockWidgetArea
                          | Qt::BottomDockWidgetArea | Qt::TopDockWidgetArea);

    dock->setWidget(m_connectedDevicesList);
    addDockWidget(Qt::RightDockWidgetArea, dock);
    m_viewMenu->addAction(dock->toggleViewAction());
    dock->setStyleSheet(styleSheet);
    dock->toggleViewAction()->setDisabled(true);
    dock->setVisible(false);
}


void MainWindow::connectActionsToSlots()
{
    QObject::connect(m_openHelpAct, SIGNAL(triggered(bool)), this, SLOT(openHelp()));
    QObject::connect(m_openFileAct, SIGNAL(triggered(bool)), this, SLOT(openFileOrProject()));
    QObject::connect(m_saveAct, SIGNAL(triggered(bool)), m_roboIdeTextEdit, SLOT(saveFile()));
    QObject::connect(m_saveAsAct, SIGNAL(triggered(bool)), m_roboIdeTextEdit, SLOT(saveFileAs()));
    QObject::connect(m_redoAct, SIGNAL(triggered(bool)), m_roboIdeTextEdit, SLOT(redo()));
    QObject::connect(m_undoAct, SIGNAL(triggered(bool)), m_roboIdeTextEdit, SLOT(undo()));
    QObject::connect(m_copyAct, SIGNAL(triggered(bool)), m_roboIdeTextEdit, SLOT(copy()));
    QObject::connect(m_pasteAct, SIGNAL(triggered(bool)), m_roboIdeTextEdit, SLOT(paste()));
    QObject::connect(m_newFileAct, SIGNAL(triggered(bool)), m_roboIdeTextEdit, SLOT(blankFile()));
    //QObject::connect(m_findAct, SIGNAL(triggered(bool)), m_roboIdeTextEdit, SLOT(()));
    QObject::connect(m_buildAct, SIGNAL(triggered(bool)), this, SLOT(runCompilation()));
    QObject::connect(m_sourceCompiller, SIGNAL(onCompilationOutput(QString)), m_roboIdeConsole, SLOT(append(QString)));
    QObject::connect(m_sourceCompiller, SIGNAL(finished()), this, SLOT(compilationFinished()));
    QObject::connect(m_uploadAndRunAct, SIGNAL(triggered(bool)), this, SLOT(uploadAndRun()));
    QObject::connect(m_showSettingsAct, SIGNAL(triggered(bool)), m_settingsWidget, SLOT(show()));
    QObject::connect(m_wifiConnection, SIGNAL(onExecOutput(QString)), m_roboIdeConsole, SLOT(append(QString)));
    QObject::connect(m_runAppAct, SIGNAL(triggered(bool)), this, SLOT(runApp()));
    QObject::connect(m_stopAppAct, SIGNAL(triggered(bool)), this, SLOT(killApp()));
    QObject::connect(m_edisonCompileAct, SIGNAL(triggered(bool)), this, SLOT(switchCompilationTargetToEdison()));
    QObject::connect(m_mingwCompileAct, SIGNAL(triggered(bool)), this, SLOT(switchCompilationTargetToDesktop()));

    QObject::connect(m_localApp, SIGNAL(readyReadStandardError()), SLOT(processOutReceived()));
    QObject::connect(m_localApp, SIGNAL(readyReadStandardOutput()), SLOT(processOutReceived()));
    QObject::connect(m_localApp, SIGNAL(readyRead()), SLOT(processOutReceived()));
}

