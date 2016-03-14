#include "projectmanager.h"

#include <QFile>
#include <QMessageBox>
#include <QDebug>

ProjectManager &ProjectManager::instance()
{
    static ProjectManager instance;
    return instance;
}

Project* ProjectManager::getProject() const
{
    return m_project;
}

void ProjectManager::createFile(QString name, QString path)
{
    QString fullPath = path + "/" + name;
    QFile fileToSave(fullPath);
    if (!fileToSave.open(QIODevice::WriteOnly | QFile::Truncate | QIODevice::Text))
    {
        qWarning() << "Something goes wrong at: ProjectManager::saveFile";
        return;
    }
    fileToSave.close();

    emit fileCreated();

    if (m_project->isOpened()) {
        // add to project file
        m_project->addFile(name);
    }
}

void ProjectManager::newFileOrProject()
{
    m_projectManagerWindow->projectOpenMode(m_project->isOpened());
    m_projectManagerWindow->setModal(true);
    m_projectManagerWindow->show();
}

void ProjectManager::openProject(QString fileName)
{
    closeProject(!m_project->isOpened());
    m_project->openProject(fileName);
}

void ProjectManager::closeProject(bool isOpening)
{
    if (!isOpening) {
        if (m_project->hasUnsavedChanges()) {
            int answer = QMessageBox::warning(this, tr("Изменения в проекте"),
                                              tr("Сохранить изменения в проекте %1").arg(m_project->getProjectName()),
                                              QMessageBox::Yes | QMessageBox::No);
            if (answer == QMessageBox::Yes) {
                m_project->saveChanges();
            }
            else {
                m_project->resetChanges();
            }
        }

        m_project->closeProject();
    }
}

void ProjectManager::projectCreated()
{
    m_project->addFile("main.cpp");
    emit onProjectCreated();
}

ProjectManager::ProjectManager(QWidget *parent) :
    QWidget(parent),
    m_project(new Project(this)),
    m_projectManagerWindow(new ProjectManagerWindow())
{
    QObject::connect(m_projectManagerWindow, SIGNAL(createProject(QString,QString)), m_project, SLOT(create(QString,QString)));
    QObject::connect(m_projectManagerWindow, SIGNAL(createFile(QString,QString)),
                     this, SLOT(createFile(QString,QString)));
    QObject::connect(m_projectManagerWindow, SIGNAL(projectCreated()), this, SLOT(projectCreated()));
}

ProjectManager::~ProjectManager()
{
    delete m_project;
    delete m_projectManagerWindow;
}
