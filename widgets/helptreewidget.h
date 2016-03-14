#ifndef HELPTREEWIDGET_H
#define HELPTREEWIDGET_H

#include <QWidget>
#include <QTreeWidget>

class HelpTreeWidget : public QWidget
{
    Q_OBJECT
public:
    explicit HelpTreeWidget(QWidget *parent = 0);
    void openJsonDocument(const QString& filepath);
signals:
    void itemClicked(QString);
private:
    void parseJson();
private slots:
    void itemDoubleClicked(QTreeWidgetItem *item, int num);
private:
    QTreeWidget *m_tree;
};

#endif // HELPTREEWIDGET_H
