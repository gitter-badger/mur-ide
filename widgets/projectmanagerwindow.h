#ifndef PROJECTMANAGERWINDOW_H
#define PROJECTMANAGERWINDOW_H

#include <QLabel>
#include <QDialog>
#include <QWidget>
#include <QLineEdit>
#include <QPushButton>
#include <QGridLayout>
#include <QListWidget>

class ProjectManagerWindow : public QDialog
{
    Q_OBJECT

public:
    explicit ProjectManagerWindow(QWidget *parent = 0);
    virtual ~ProjectManagerWindow();
    void projectOpenMode(bool status);

signals:
    void createProject(QString, QString);
    void createFile(QString, QString);
    void projectCreated();
private slots:
    void onLoadClicked();
    void onCreateClicked();

private:
    QGridLayout* m_layout;
    QLineEdit* m_nameLine;
    QLineEdit* m_pathLine;
    QPushButton* m_loadPath;
    QPushButton* m_createProject;
    QListWidget* m_listWidget;
    QMap<QListWidgetItem*, QString> m_extension;
};

#endif // PROJECTMANAGERWINDOW
