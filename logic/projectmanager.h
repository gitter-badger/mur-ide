#ifndef PROJECTMANAGER_H
#define PROJECTMANAGER_H

#include "project.h"
#include "widgets/projectmanagerwindow.h"

#include <QObject>

class ProjectManager : public QWidget
{
    Q_OBJECT
public:
    static ProjectManager& instance();

    Project* getProject() const;

signals:
    void fileCreated();
    void onProjectCreated();
public slots:
    void newFileOrProject();
    void createFile(QString name, QString path);
    void openProject(QString fileName);
    void closeProject(bool isOpening);
    void projectCreated();
private:
    explicit ProjectManager(QWidget *parent = 0);
    virtual ~ProjectManager();
    ProjectManager(ProjectManager const&) = delete;
    void operator= (ProjectManager const&) = delete;

    Project *m_project;
    ProjectManagerWindow *m_projectManagerWindow;
};

#endif // PROJECTMANAGER_H
