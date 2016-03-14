#include <QTreeWidgetItem>
#include <QJsonArray>
#include <QJsonObject>
#include <QJsonDocument>
#include <QByteArray>
#include <QFile>
#include <QLabel>
#include <QDebug>

#include "helptreewidget.h"
#include <QVBoxLayout>

HelpTreeWidget::HelpTreeWidget(QWidget *parent) :
    QWidget(parent),
    m_tree(new QTreeWidget(this))
{
    QVBoxLayout *mainLayout = new QVBoxLayout(this);
    setLayout(mainLayout);
    mainLayout->addWidget(m_tree);
    m_tree->setHeaderLabel("Функции");
    m_tree->setExpandsOnDoubleClick(false);
    QObject::connect(m_tree, SIGNAL(itemDoubleClicked(QTreeWidgetItem*,int)), this, SLOT(itemDoubleClicked(QTreeWidgetItem*,int)));
}

void HelpTreeWidget::openJsonDocument(const QString &filepath)
{
    QJsonDocument doc;
    QFile helpFile(filepath);
    if (!helpFile.open(QIODevice::ReadOnly)) {
        qWarning("Could not open pro file");
        return;
    }
    QByteArray jData = helpFile.readAll();
    helpFile.close();

    doc = QJsonDocument::fromJson(jData);
    QJsonObject obj = doc.object();
    QJsonArray jarray = obj["help"].toArray();

    for (int i = 0; i < jarray.size(); i++)
    {
        QString itemName = jarray.at(i).toObject()["name"].toString();
        QString itemDdsc = jarray.at(i).toObject()["dsc"].toString();

        QTreeWidgetItem *name = new QTreeWidgetItem();
        QTreeWidgetItem *dsc = new QTreeWidgetItem();
        name->setData(0, Qt::DisplayRole, itemName);
        dsc->setData(0, Qt::DisplayRole, itemDdsc);
        name->addChild(dsc);
        m_tree->addTopLevelItem(name);
    }
}

void HelpTreeWidget::itemDoubleClicked(QTreeWidgetItem *item, int num)
{
    Q_UNUSED(num)
    if (item->childCount() != 0) {
        emit itemClicked(item->data(0, Qt::DisplayRole).toString());
    }

}

