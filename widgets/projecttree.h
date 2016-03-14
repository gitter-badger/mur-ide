#ifndef PROJECTTREE_H
#define PROJECTTREE_H

#include <QWidget>
#include <QAction>
#include <QHBoxLayout>
#include <QTreeWidget>

class ProjectTree : public QWidget
{
    Q_OBJECT
public:
    explicit ProjectTree(QWidget *parent = 0);
    virtual ~ProjectTree();

signals:
    void showContent(QString, QString);
    void showFileContent(QString);

public slots:
    void onContentChanged(QString text);
    void onFileSaved();

private slots:
    void onItemClicked(QTreeWidgetItem* item, int column);
    void onCustomContextMenu(QPoint point);
    void deleteItem();

public slots:
    void projectChanged();
    void clear();

private:
    QAction* m_deleteAction;
    QHBoxLayout* m_mainLayout;
    QTreeWidget* m_treeWidget;
    QTreeWidgetItem* m_sourcesLabel;
    QTreeWidgetItem* m_headersLabel;
    QMap<QString, QTreeWidgetItem *> m_headerItems;
    QMap<QString, QTreeWidgetItem *> m_sourceItems;
    QMap<QTreeWidgetItem*, QString> m_allItems;
    QMap<QTreeWidgetItem*, QString> m_changedContent;
    QTreeWidgetItem* m_currentItem = nullptr;
    QTreeWidgetItem* m_deleteIndex;

    void addItems(QMap<QString, QTreeWidgetItem*> &currentItems, QTreeWidgetItem* parentItem,
                 QVariantList children, QIcon icon);
    void removeChildrenItems(QTreeWidgetItem *parent);
};

#endif // PROJECTTREE_H
