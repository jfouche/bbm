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
        data.addProject(json::read(projObj, "name"));
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
    obj["name"] = project.name;
}

DataModel::DataModel(QObject *parent) : QObject(parent)
{
    m_projects.push_back(Project("Project1"));
    m_projects.push_back(Project("Project2"));
    m_projects.push_back(Project("Project3"));
    m_projects.push_back(Project("Project4"));
}

const Project* DataModel::addProject(const QString &name)
{
    if (getProject(name) != nullptr) {
        return nullptr;
    }
    m_projects.push_back(Project(name));
    emit dbChanged();
    return &(*m_projects.rbegin());
}

const BuildingBlock* DataModel::addBuildingBlock(const QString& name, const QString& ref)
{
    if (getBuildingBlock(ref) != nullptr) {
        return nullptr;
    }
    m_buildingblocks.push_back(BuildingBlock(name, ref));
    emit dbChanged();
    return &(*m_buildingblocks.rbegin());
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
    return &m_projects.at(index);
}

const Project* DataModel::getProject(const QString& name) const
{
    auto by_name = [name](const Project& p) {
        return p.name == name;
    };
    auto it = std::find_if(m_projects.begin(), m_projects.end(), by_name);
    return (it != m_projects.end()) ? &(*it) : nullptr;
}

const BuildingBlock* DataModel::getBuildingBlock(const QString& ref) const
{
    auto by_ref = [ref](const BuildingBlock& bb) {
        return bb.ref == ref;
    };
    auto it = std::find_if(m_buildingblocks.begin(), m_buildingblocks.end(), by_ref);
    return (it != m_buildingblocks.end()) ? &(*it) : nullptr;
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
        write(project, projectObj);
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
