#include "datamodel.h"
#include "datamodel_file.h"
#include <QFile>
#include <QJsonDocument>
#include <QJsonObject>
#include <QDebug>


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

const QVector<Project*>& DataModel::projects()
{
    return m_projects;
}

DataModel::ProjectsView DataModel::projects() const
{
    return ProjectsView(m_projects);
}

const QVector<BuildingBlock*>& DataModel::buildingBlocks()
{
    return m_buildingblocks;
}

DataModel::BuildingBlocksView DataModel::buildingBlocks() const
{
    return BuildingBlocksView(m_buildingblocks);
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
    emit modelChanged();
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
