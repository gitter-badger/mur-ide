#include "connecteddeviceslist.h"

#include <QGridLayout>
#include <QLayout>
#include <QLabel>
#include <QIcon>
#include <QPixmap>

ConnectedDevicesList::ConnectedDevicesList(QWidget *parent)
    : QWidget(parent)
{
    setMaximumSize(230, 150);
    QGridLayout *mainLayout = new QGridLayout(this);
    setLayout(mainLayout);

    mainLayout->addWidget(createHeaderLabel("Порт 1"), 0, 0);
    mainLayout->addWidget(createPixmapLabel(), 1, 0);
    mainLayout->addWidget(createHeaderLabel("Порт 2"), 0, 1);
    mainLayout->addWidget(createPixmapLabel(), 1, 1);
    mainLayout->addWidget(createHeaderLabel("Порт 3"), 0, 2);
    mainLayout->addWidget(createPixmapLabel(), 1, 2);
    mainLayout->addWidget(createHeaderLabel("Порт 4"), 0, 3);
    mainLayout->addWidget(createPixmapLabel(), 1, 3);

    mainLayout->addWidget(createHeaderLabel("Порт 5"), 2, 0);
    mainLayout->addWidget(createPixmapLabel(), 3, 0);
    mainLayout->addWidget(createHeaderLabel("Порт 6"), 2, 1);
    mainLayout->addWidget(createPixmapLabel(), 3, 1);
    mainLayout->addWidget(createHeaderLabel("Порт 7"), 2, 2);
    mainLayout->addWidget(createPixmapLabel(), 3, 2);
    mainLayout->addWidget(createHeaderLabel("Порт 8"), 2, 3);
    mainLayout->addWidget(createPixmapLabel(), 3, 3);
}

QLabel *ConnectedDevicesList::createHeaderLabel(const QString &text)
{
    QLabel *label = new QLabel(text, this);
    label->setAlignment(Qt::AlignCenter);
    return label;
}

QLabel *ConnectedDevicesList::createPixmapLabel()
{
    QLabel *label = new QLabel(this);
    label->setEnabled(false);
    label->setAlignment(Qt::AlignCenter);
    label->setFrameShape(QFrame::Box);
    label->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    label->setBackgroundRole(QPalette::Base);
    label->setAutoFillBackground(true);
    label->setMinimumSize(48, 48);
    label->setMaximumSize(48, 48);
    return label;
}


