#ifndef PROJECT_H
#define PROJECT_H

#include <QMap>
#include <QWidget>
#include <QJsonArray>
#include <QJsonObject>
#include <QJsonDocument>

class Project : public QWidget
{
    Q_OBJECT

public:
    Project(QWidget* parent = 0);
    virtual ~Project();

    void addFile(QString name);
    void deleteFile(QString name);
    void generateMakeFile(QString compilatorPath, QString sysrootPath, QString options);
    void closeProject();
    bool isOpened() const;

    QString getValueByName(QString name) const;
    QString getProjectName() const;
    QString getProjectLocation() const;
    QVariantList getHeaderFiles() const;
    QVariantList getSourceFiles() const;

    void saveChanges();
    void resetChanges();
    bool hasUnsavedChanges() const;
    void contentSaved(QString fileName);
    void contentUnsaved(QString fileName, QString content);

signals:
    void projectOpened(bool);
    void projectClosed();
    void projectChanged();

public slots:
    void openProject(QString fileName);

private slots:
    void create(QString name, QString location);

private:
    bool m_isOpened = false;
    QString m_name;
    QString m_location;
    QJsonDocument m_proFile;
    QMap<QString, QString> m_unsavedChanges;

    void saveProFile(QString location);
    void deleteJsonArrayItem(QJsonObject &object, QString arrayName, QString itemName);
};

#endif // PROJECT_H
