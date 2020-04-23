#include "datamodel.h"
#include "json_utils.hpp"
#include <QFile>
#include <QJsonDocument>
#include <QDebug>

static void read(DataModel& data, const QJsonObject& jsonObj)
{
    auto value = json::check_exists(jsonObj, "projects");
    const QJsonArray array = json::check_type<QJsonArray>(value);
    for (auto it = array.begin(); it != array.end(); ++it) {
        QJsonObject projObj = json::check_type<QJsonObject>(json::json_pair("project index", *it));
        auto project =  data.addProject();
        project->setName(json::read(projObj, "name"));
    }
    if (jsonObj.contains("projects") && jsonObj["projects"].isObject()) {

    }
}

/*
static void read(Project& project, const QJsonObject& jsonObj)
{
    std::function<void(QString)> cb = [&project](QString value) {
        project.name = value;
    };
    project.name = json::read(jsonObj, "name");
}
*/

static void write(const Project& project, QJsonObject& obj)
{
    obj["name"] = project.name();
}

// ===========================================================================
Project::Project(QObject* parent)
    : QObject(parent)
{
}

void Project::setName(const QString &name)
{
    m_name = name;
    emit changed();
}


// ===========================================================================
BuildingBlock::BuildingBlock(QObject* parent)
    : QObject(parent)
{
}

void BuildingBlock::setName(const QString &name)
{
    m_name = name;
    emit changed();
}

void BuildingBlock::setRef(const QString &ref)
{
    m_ref = ref;
    emit changed();
}


// ===========================================================================
DataModel::DataModel(QObject *parent)
    : QObject(parent)
{
}

Project* DataModel::addProject()
{
//    if (getProject(name) != nullptr) {
//        return nullptr;
//    }
    auto project = new Project(this);
    connect(project, SIGNAL(changed()), this, SLOT(changed()));
    m_projects.push_back(project);
    emit dbChanged();
    return project;
}

BuildingBlock* DataModel::addBuildingBlock()
{
//    if (getBuildingBlock(ref) != nullptr) {
//        return nullptr;
//    }
    auto bb = new BuildingBlock(this);
    connect(bb, SIGNAL(changed()), this, SLOT(changed()));
    m_buildingblocks.push_back(bb);
    emit dbChanged();
    return bb;
}

void DataModel::changed()
{
    emit dbChanged();
}

int DataModel::getProjectCount() const
{
    return m_projects.size();
}

const Project* DataModel::getProject(int index) const
{
    if (index > m_projects.size()) {
        return nullptr;
    }
    return m_projects.at(index);
}

Project* DataModel::getProject(int index)
{
    if (index > m_projects.size()) {
        return nullptr;
    }
    return m_projects.at(index);
}

void DataModel::save(const QString& path) const
{
    QFile file(path);

    if (!file.open(QIODevice::WriteOnly)) {
        qWarning("Couldn't open save file.");
        return;
    }

    QJsonArray projects;
    for (auto project : m_projects) {
        QJsonObject projectObj;
        write(*project, projectObj);
        projects.append(projectObj);
    }

    QJsonObject root;
    root["projects"] = projects;
    QJsonDocument doc(root);
    file.write(doc.toJson());
}

void DataModel::load(const QString& path)
{
    QFile file(path);
    if (!file.open(QIODevice::ReadOnly)) {
        qWarning("Couldn't open save file.");
        return;
    }
    QByteArray fileContent = file.readAll();
    QJsonDocument doc(QJsonDocument::fromJson(fileContent));
    try {
        read(*this, doc.object());
    }
    catch (std::exception& e) {
        qWarning() << e.what();
    }
}
