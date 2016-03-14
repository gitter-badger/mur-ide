#include "projectmanagerwindow.h"
#include "logic/projectmanager.h"

#include <QFileDialog>

ProjectManagerWindow::ProjectManagerWindow(QWidget *parent) :
    QDialog(parent)
{
    m_listWidget = new QListWidget(this);
    m_extension[new QListWidgetItem(QIcon(":/icons/actions/edit-redo.png"), tr("Заголовочный файл"), m_listWidget)] = ".h";
    m_extension[new QListWidgetItem(QIcon(":/icons/actions/edit-undo.png"), tr("Исходный файл"), m_listWidget)] = ".cpp";
    m_extension[new QListWidgetItem(QIcon(":/icons/actions/document-save.png"), tr("Проект"), m_listWidget)] = ".robopro";

    m_listWidget->setCurrentRow(0);
    m_listWidget->setFixedHeight(m_listWidget->contentsRect().height() * m_listWidget->count());

    m_nameLine = new QLineEdit("undefined", this);
    m_pathLine = new QLineEdit(this);
    m_loadPath = new QPushButton(tr("Обзор"), this);
    m_createProject = new QPushButton(tr("Создать"), this);

    m_layout = new QGridLayout(this);
    m_layout->addWidget(m_listWidget, 1, 0, 1, -1);
    m_layout->addWidget(new QLabel(tr("Название"), this), 2, 0);
    m_layout->addWidget(m_nameLine, 2, 1, 1, -1);
    m_layout->addWidget(new QLabel(tr("Расположение"), this), 3, 0);
    m_layout->addWidget(m_pathLine, 3, 1);
    m_layout->addWidget(m_loadPath, 3, 2);
    m_layout->addWidget(m_createProject, 4, 2, Qt::AlignRight);

    //!Diable "?" button
    setWindowFlags(windowFlags() & ~Qt::WindowContextHelpButtonHint);

    QObject::connect(m_loadPath, SIGNAL(clicked(bool)), this, SLOT(onLoadClicked()));
    QObject::connect(m_createProject, SIGNAL(clicked(bool)), this, SLOT(onCreateClicked()));
}

ProjectManagerWindow::~ProjectManagerWindow()
{
}

void ProjectManagerWindow::projectOpenMode(bool status)
{
    m_loadPath->setDisabled(status);
    m_pathLine->setDisabled(status);
    if (status) {
        m_pathLine->setText(ProjectManager::instance().getProject()->getProjectLocation());
    }
    QList<QListWidgetItem *> projectItems = m_listWidget->findItems(tr("Проект"), Qt::MatchExactly);
    if (projectItems.size()) {
        projectItems.at(0)->setHidden(status);
    }
}

void ProjectManagerWindow::onLoadClicked()
{
    QString dir = QFileDialog::getExistingDirectory(this);
    if (!dir.isEmpty()) {
        m_pathLine->setText(dir);
    }
}

void ProjectManagerWindow::onCreateClicked()
{
    if (!(m_nameLine->text().isEmpty()) && !(m_pathLine->text().isEmpty())) {
        QString fileExtension = m_extension[m_listWidget->currentItem()];
        if (fileExtension == ".robopro") {
            emit createProject(m_nameLine->text(), m_pathLine->text());
            emit createFile("main.cpp", m_pathLine->text());
            emit projectCreated();
        }
        else {
            emit createFile(m_nameLine->text() + fileExtension, m_pathLine->text());
        }

        m_nameLine->setText("undefined");
        this->close();
    }
}
