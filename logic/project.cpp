#include "project.h"

#include <QDebug>
#include <QFileInfo>
#include <QFileDialog>

Project::Project(QWidget *parent) :
    QWidget(parent)
{

}

Project::~Project()
{

}

void Project::addFile(QString name)
{
    QJsonObject newProFile = m_proFile.object();
    if (name.endsWith(".h")) {
        QJsonArray headers = m_proFile.object()["headers"].toArray();
        headers.append(name);
        newProFile["headers"] = headers;
    }
    else if (name.endsWith(".cpp")) {
        QJsonArray sources = m_proFile.object()["sources"].toArray();
        sources.append(name);
        newProFile["sources"] = sources;
    }
    m_proFile = QJsonDocument(newProFile);
    saveProFile(m_location);
    emit projectChanged();
}

void Project::deleteJsonArrayItem(QJsonObject &object, QString arrayName, QString itemName)
{
    QJsonArray array = object[arrayName].toArray();
    for (int i = 0; i < array.size(); ++i) {
        if (array.at(i) == itemName) {
            array.removeAt(i);
            object[arrayName] = array;
            break;
        }
    }
}

void Project::deleteFile(QString name)
{
    QJsonObject proFile = m_proFile.object();
    if (name.endsWith(".h")) {
        deleteJsonArrayItem(proFile, "headers", name);
    }
    else if (name.endsWith(".cpp")) {
        deleteJsonArrayItem(proFile, "sources", name);
    }

    m_proFile = QJsonDocument(proFile);
    saveProFile(m_location);
}

void Project::generateMakeFile(QString compilatorPath, QString sysrootPath, QString options)
{
    QString makefilePath = m_location + "/makefile";
    QFile makeFile(makefilePath);
    if (!makeFile.open(QIODevice::WriteOnly)) {
        qWarning("Could not open makefile");
        return;
    }

    QTextStream makefile(&makeFile);
    makefile << QString("CC=%1\n").arg(compilatorPath);
    makefile << QString("%1:\n").arg(getProjectName());
    makefile << QString("\t$(CC) --sysroot=%1 ").arg(sysrootPath);
    QVariantList sourceFiles = getSourceFiles();
    for (auto source : sourceFiles) {
        makefile << source.toString() << " ";
    }
    makefile << QString("-o %1.bin ").arg(getProjectName());
    makefile << options;

    makeFile.close();
}

void Project::closeProject()
{
    m_isOpened = false;
    emit projectClosed();
}

void Project::saveProFile(QString location)
{
    QString fullPath = location + "/" + m_name + ".robopro";
    QFile proFile(fullPath);
    if (!proFile.open(QIODevice::WriteOnly)) {
        qWarning("Could not open pro file");
        return;
    }

    proFile.write(m_proFile.toJson());
    proFile.close();
}

bool Project::isOpened() const
{
    return m_isOpened;
}

QString Project::getValueByName(QString name) const
{
    QJsonObject jsonObject = m_proFile.object();
    QJsonValue value = jsonObject.take(name);
    if (value == QJsonValue::Undefined) {
        qWarning() <<  name << " is not set";
        return QString();
    }

    return value.toString();
}

QString Project::getProjectLocation() const
{
    return getValueByName("location");
}

QString Project::getProjectName() const
{
    return getValueByName("name");
}

QVariantList Project::getHeaderFiles() const
{
    QJsonObject jsonObject = m_proFile.object();
    QJsonArray headerFiles = jsonObject["headers"].toArray();
    return headerFiles.toVariantList();
}

QVariantList Project::getSourceFiles() const
{
    QJsonObject jsonObject = m_proFile.object();
    QJsonArray sourceFiles = jsonObject["sources"].toArray();
    return sourceFiles.toVariantList();
}

void Project::saveChanges()
{
    for (auto file : m_unsavedChanges.keys()) {
        QFile fileToSave(m_location + "/" + file);
        if (!fileToSave.open(QIODevice::WriteOnly | QFile::Truncate | QIODevice::Text)) {
            qWarning() << "Something goes wrong at: Project::saveChanges() [fileToSave.open(...)]";
            return;
        }

        QTextStream out(&fileToSave);
        out.setCodec("UTF-8");
        out << m_unsavedChanges.value(file);
        fileToSave.close();
    }

    resetChanges();
}

void Project::resetChanges()
{
    m_unsavedChanges.clear();
}

bool Project::hasUnsavedChanges() const
{
    return m_unsavedChanges.size() ? true : false;
}

void Project::contentSaved(QString fileName)
{
    if (m_unsavedChanges.count(fileName)) {
        m_unsavedChanges.remove(fileName);
    }
}

void Project::contentUnsaved(QString fileName, QString content)
{
    m_unsavedChanges[fileName] = content;
}

void Project::create(QString name, QString location)
{
    m_name = name;
    m_location = location;

    QJsonObject proFileTemplate;
    proFileTemplate["name"] = name;
    proFileTemplate["location"] = location;
    proFileTemplate["headers"] = QJsonArray();
    proFileTemplate["sources"] = QJsonArray();

    QJsonDocument saveInfo(proFileTemplate);
    m_proFile = saveInfo;
    saveProFile(m_location);
    m_isOpened = true;

    emit projectChanged();
    emit projectOpened(true);
}

void Project::openProject(QString fileName)
{
    if (m_isOpened) {
        closeProject();
    }
    QFile proFile(fileName);
    if (!proFile.open(QIODevice::ReadOnly)) {
        qWarning("Could not open pro file");
        return;
    }
    QByteArray projectStructure = proFile.readAll();
    proFile.close();

    m_proFile = QJsonDocument::fromJson(projectStructure);

    QFileInfo fileInfo(proFile);
    QString currentLocation = fileInfo.absolutePath();
    if (currentLocation != getProjectLocation()) {
        QJsonObject proFileObject = m_proFile.object();
        proFileObject["location"] = currentLocation;
        m_proFile = QJsonDocument(proFileObject);
        saveProFile(currentLocation);
    }

    m_isOpened = true;
    m_name = getProjectName();
    m_location = getProjectLocation();

    emit projectChanged();
    emit projectOpened(true);
}
