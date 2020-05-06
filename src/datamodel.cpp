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

bool Project::contains(BuildingBlock* bb) const
{
    return false;
}

// ===========================================================================
BuildingBlock::BuildingBlock(QObject* parent)
    : QObject(parent)
    , m_maturity(F)
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

void BuildingBlock::setInfo(const QString &info)
{
    m_info = info;
    emit changed();
}

void BuildingBlock::setMaturity(Maturity maturity)
{
    m_maturity = maturity;
    emit changed();
}

void BuildingBlock::add(BuildingBlock *bb)
{
    int index = m_children.indexOf(bb);
    if (index == -1) {
        m_children.append(bb);
        emit childAdded(bb, this);
    }
}

void BuildingBlock::remove(BuildingBlock* bb)
{
    int index = m_children.indexOf(bb);
    if (index != -1) {
        m_children.removeAt(index);
        emit childRemoved(bb, this);
    }
}

const QList<BuildingBlock *> &BuildingBlock::children()
{
    return m_children;
}

const QList<BuildingBlock *> &BuildingBlock::children() const
{
    return m_children;
}

bool BuildingBlock::contains(BuildingBlock* bb) const
{
    for (const BuildingBlock* child : m_children) {
        if (child == bb) {
            return true;
        }
        if (child->contains(bb)) {
            return true;
        }
    }
    return false;
}

// ===========================================================================
DataModel::DataModel(QObject *parent)
    : QObject(parent)
{
}


const QVector<Project*>& DataModel::projects() const
{
    return m_projects;
}

const QVector<BuildingBlock*>& DataModel::buildingBlocks() const
{
    return m_buildingblocks;
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

bool DataModel::removeBuildingBlock(BuildingBlock* bb)
{
    const int index = m_buildingblocks.indexOf(bb);
    Q_ASSERT(index != -1);

    // Search recursively in projects if it is use
    for (const Project* project : m_projects) {
        if (project->contains(bb)) {
            return false;
        }
    }

    // Search recursively in building blocks if it is use
    for (const BuildingBlock* child : m_buildingblocks) {
        if (child->contains(bb)) {
            return false;
        }
    }

    // Remove the BB
    delete m_buildingblocks.at(index);
    m_buildingblocks.removeAt(index);
    emit buildingBlockRemoved(bb);
    return true;
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
