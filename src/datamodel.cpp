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

static void write(const BuildingBlock& bb, QJsonObject& obj)
{
    obj["name"] = bb.name();
    obj["ref"] = bb.ref();
}

static void write(const DataModel& model, QJsonObject& obj)
{
    QJsonArray projects;
    for (int i = 0; i < model.getProjectCount(); ++i) {
        QJsonObject projectObj;
        const Project* project = model.getProject(i);
        if (project) {
            write(*project, projectObj);
            projects.append(projectObj);
        }
    }
    obj["projects"] = projects;

    QJsonArray buildingblocks;
    for (int i = 0; i < model.getBuildingBlockCount(); ++i) {
        QJsonObject bbObj;
        const BuildingBlock* bb = model.getBuildingBlock(i);
        if (bb) {
            write(*bb, bbObj);
            projects.append(bbObj);
        }
    }
    obj["buildingblocks"] = projects;
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
    auto project = new Project(this);
    connect(project, SIGNAL(changed()), this, SLOT(changed()));
    m_projects.push_back(project);
    emit projectAdded(project);
    return project;
}

BuildingBlock* DataModel::addBuildingBlock()
{
    auto bb = new BuildingBlock(this);
    connect(bb, SIGNAL(changed()), this, SLOT(changed()));
    m_buildingblocks.push_back(bb);
    emit buildingBlockAdded(bb);
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

int DataModel::getBuildingBlockCount() const
{
    return m_buildingblocks.size();
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

const BuildingBlock* DataModel::getBuildingBlock(int index) const
{
    if (index > m_buildingblocks.size()) {
        return nullptr;
    }
    return m_buildingblocks.at(index);
}

BuildingBlock* DataModel::getBuildingBlock(int index)
{
    if (index > m_buildingblocks.size()) {
        return nullptr;
    }
    return m_buildingblocks.at(index);
}

void DataModel::save(const QString& path) const
{
    QFile file(path);

    if (!file.open(QIODevice::WriteOnly)) {
        qWarning("Couldn't open save file.");
        return;
    }

    QJsonObject root;
    write(*this, root);
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
