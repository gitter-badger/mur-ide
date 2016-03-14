#include "projecttree.h"
#include "logic/projectmanager.h"

#include <QMenu>

ProjectTree::ProjectTree(QWidget *parent) :
    QWidget(parent)
{
    setMaximumWidth(250);
    m_mainLayout = new QHBoxLayout(this);
    setLayout(m_mainLayout);

    m_treeWidget = new QTreeWidget(this);
    m_treeWidget->setContextMenuPolicy(Qt::CustomContextMenu);

    m_treeWidget->setColumnCount(1);
    m_treeWidget->setHeaderLabel(tr("Название проекта"));

    m_sourcesLabel = new QTreeWidgetItem((QTreeWidget*)0, QStringList(QString(tr("Исходные файлы"))));
    m_sourcesLabel->setIcon(0, QIcon(":/icons/actions/edit-find.png"));
    m_treeWidget->insertTopLevelItem(0, m_sourcesLabel);

    m_headersLabel = new QTreeWidgetItem((QTreeWidget*)0, QStringList(QString(tr("Заголовочные файлы"))));
    m_headersLabel->setIcon(0, QIcon(":/icons/actions/document-new.png"));
    m_treeWidget->insertTopLevelItem(0, m_headersLabel);

    m_mainLayout->addWidget(m_treeWidget);

    m_deleteAction = new QAction(QIcon(":/icons/actions/edit-undo.png"), tr("Удалить из проекта"), this);
    QObject::connect(m_deleteAction, SIGNAL(triggered(bool)), this, SLOT(deleteItem()));

    QObject::connect(m_treeWidget, SIGNAL(itemClicked(QTreeWidgetItem*,int)), this, SLOT(onItemClicked(QTreeWidgetItem*,int)));
    QObject::connect(m_treeWidget, SIGNAL(customContextMenuRequested(QPoint)), this, SLOT(onCustomContextMenu(QPoint)));
}

ProjectTree::~ProjectTree()
{
    delete m_headersLabel;
    delete m_sourcesLabel;
}

void ProjectTree::onContentChanged(QString text)
{
    if (m_currentItem == nullptr) {
        return;
    }

    QString fileName = m_currentItem->text(0);
    if (m_changedContent.count(m_currentItem)) {
        if (m_changedContent[m_currentItem] != text) {
            ProjectManager::instance().getProject()->contentUnsaved(m_allItems[m_currentItem], text);
            if (!fileName.endsWith("*")) {
                m_currentItem->setText(0, fileName + "*");
            }
        }
    }
    m_changedContent[m_currentItem] = text;
}

void ProjectTree::onFileSaved()
{
    QString fileName = m_currentItem->text(0);
    if (fileName.endsWith("*")) {
        fileName.truncate(fileName.size() - 1);
        m_currentItem->setText(0, fileName);
        ProjectManager::instance().getProject()->contentSaved(fileName);
    }
}

void ProjectTree::onItemClicked(QTreeWidgetItem* item, int column)
{
    Q_UNUSED(column);
    if (m_allItems.count(item)) {
        m_currentItem = item;
        QString location = ProjectManager::instance().getProject()->getProjectLocation();
        QString fileName = location + "/" + m_allItems[item];
        if (m_changedContent.count(m_currentItem)) {
            emit showContent(m_changedContent[m_currentItem], fileName);
        }
        else {
            emit showFileContent(fileName);
        }
    }
}

void ProjectTree::onCustomContextMenu(QPoint point)
{
    QTreeWidgetItem* index = m_treeWidget->itemAt(point);
    if (!m_allItems.count(index)) {
        return;
    }

    m_deleteIndex = index;

    QMenu menu(this);
    menu.addAction(m_deleteAction);
    menu.exec(m_treeWidget->mapToGlobal(point));
}

void ProjectTree::deleteItem()
{
    QString fileName = m_allItems[m_deleteIndex];
    ProjectManager::instance().getProject()->deleteFile(fileName);
    m_allItems.remove(m_deleteIndex);
    if (m_headerItems.count(fileName)) {
        m_headerItems.remove(fileName);
        m_headersLabel->removeChild(m_deleteIndex);
    }
    else if (m_sourceItems.count(fileName)) {
        m_sourceItems.remove(fileName);
        m_sourcesLabel->removeChild(m_deleteIndex);
    }
}

void ProjectTree::addItems(QMap<QString, QTreeWidgetItem*> &currentItems, QTreeWidgetItem* parentItem,
                          QVariantList children, QIcon icon)
{
    foreach (QVariant file, children) {
        if (!currentItems.count(file.toString())) {
            QTreeWidgetItem* item = new QTreeWidgetItem(parentItem, QStringList(file.toString()));
            item->setIcon(0, icon);
            currentItems[file.toString()] = item;
            m_allItems[item] = file.toString();
        }
    }
    parentItem->sortChildren(0, Qt::AscendingOrder);
}

void ProjectTree::projectChanged()
{
    m_treeWidget->setHeaderLabel(ProjectManager::instance().getProject()->getProjectName());
    QVariantList headerFiles = ProjectManager::instance().getProject()->getHeaderFiles();
    QVariantList sourceFiles = ProjectManager::instance().getProject()->getSourceFiles();
    addItems(m_headerItems, m_headersLabel, headerFiles, QIcon(":/icons/actions/edit-copy.png"));
    addItems(m_sourceItems, m_sourcesLabel, sourceFiles, QIcon(":/icons/actions/tools-hammer.png"));
}

void ProjectTree::removeChildrenItems(QTreeWidgetItem *parent)
{
    QList<QTreeWidgetItem *> items = parent->takeChildren();
    for (auto item : items) {
        parent->removeChild(item);
    }
}

void ProjectTree::clear()
{
    m_allItems.clear();
    m_headerItems.clear();
    m_sourceItems.clear();
    m_changedContent.clear();

    removeChildrenItems(m_headersLabel);
    removeChildrenItems(m_sourcesLabel);

    m_currentItem = nullptr;
}
