#include "compilersettingswidget.h"
#include <QLabel>
#include <QGridLayout>
#include <QDebug>

/*
    QLineEdit *m_ccEdit;
    QLineEdit *m_sysrootEdit;
    QLineEdit *m_compilerPathEdit;
    QLineEdit *m_compilerOptEdit;
*/
CompilerSettingsWidget::CompilerSettingsWidget(QWidget *parent) :
    QWidget(parent),
    m_sysrootEdit(new QLineEdit(this)),
    m_compilerPathEdit(new QLineEdit(this)),
    m_compilerOptEdit(new QLineEdit(this)),
    m_mingwMakePathEdit(new QLineEdit(this))
{
    QGridLayout *layout = new QGridLayout(this);
    setLayout(layout);

    QLabel *sysrootLabel = new QLabel("Путь к sysroot: ", this);
    QLabel *compilerPathLabel = new QLabel("Путь к компилятору: ", this);
    QLabel *compilerOptLabel = new QLabel("Опции компилятора: ", this);
    QLabel *mingwMakePathLabel = new QLabel("Путь к программе make: ", this);

    layout->addWidget(sysrootLabel, 0, 0);
    layout->addWidget(compilerPathLabel, 1, 0);
    layout->addWidget(compilerOptLabel, 2, 0);
    layout->addWidget(mingwMakePathLabel, 3, 0);

    layout->addWidget(m_sysrootEdit, 0, 1);
    layout->addWidget(m_compilerPathEdit, 1, 1);
    layout->addWidget(m_compilerOptEdit, 2, 1);
    layout->addWidget(m_mingwMakePathEdit, 3, 1);
}

void CompilerSettingsWidget::setCompilationSettings(const QString &ccPath, const QString &sysrootPath,
                                                    const QString &ccOpt, const QString &makePath)
{
    m_sysrootEdit->setText(sysrootPath);
    m_compilerOptEdit->setText(ccOpt);
    m_compilerPathEdit->setText(ccPath);
    m_mingwMakePathEdit->setText(makePath);
}

QString CompilerSettingsWidget::sysrootPath()
{
    return m_sysrootEdit->text();
}

QString CompilerSettingsWidget::compilerOpt()
{
    return m_compilerOptEdit->text();
}

QString CompilerSettingsWidget::compilerPath()
{
    return m_compilerPathEdit->text();
}

QString CompilerSettingsWidget::mingwMakePath()
{
    return m_mingwMakePathEdit->text();
}
