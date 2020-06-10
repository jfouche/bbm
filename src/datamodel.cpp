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

const QList<BuildingBlock*>& Project::buildingBlocks() const
{
    return m_listBb;
}

void Project::setName(const QString &name)
{
    if (name != m_name) {
        m_name = name;
        emit changed();
    }
}

bool Project::contains(BuildingBlock* bb) const
{
    return m_listBb.contains(bb);
}

void Project::add(BuildingBlock* bb)
{
    if (m_listBb.contains(bb) == false) {
        m_listBb.push_back(bb);
        emit bbAdded(bb);
        bb->addParent(this);
    }
}

void Project::remove(BuildingBlock* bb)
{
    int index = m_listBb.indexOf(bb);
    if (index != -1) {
        m_listBb.removeAt(index);
        emit bbRemoved(bb);
        bb->removeParent(this);
    }
}

// ===========================================================================
BuildingBlock::BuildingBlock(QObject* parent)
    : QObject(parent)
    , m_maturity(F)
{
}

void BuildingBlock::setName(const QString &name)
{
    if (name != m_name) {
        m_name = name;
        emit changed();
    }
}

void BuildingBlock::setRef(const QString &ref)
{
    if (ref != m_ref) {
        m_ref = ref;
        emit changed();
    }
}

void BuildingBlock::setInfo(const QString &info)
{
    if (info != m_info) {
        m_info = info;
        emit changed();
    }
}

void BuildingBlock::setMaturity(Maturity maturity)
{
    if (maturity != m_maturity) {
        m_maturity = maturity;
        emit changed();
    }
}

void BuildingBlock::add(BuildingBlock *bb)
{
    if (m_children.contains(bb) == false) {
        m_children.append(bb);
        emit childAdded(bb);
        bb->addParent(this);
    }
}

void BuildingBlock::remove(BuildingBlock* bb)
{
    int index = m_children.indexOf(bb);
    if (index != -1) {
        m_children.removeAt(index);
        emit childRemoved(bb);
        bb->removeParent(this);
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

const QList<BuildingBlock*>& BuildingBlock::parentBb()
{
    return m_parentBb;
}

const QList<Project*>& BuildingBlock::parentProject()
{
    return m_parentProjects;
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

void BuildingBlock::addParent(Project* project)
{
    Q_ASSERT(m_parentProjects.indexOf(project) == -1);
    if (m_parentProjects.indexOf(project) == -1) {
        m_parentProjects.push_back(project);
        emit parentAdded(project);
    }
}

void BuildingBlock::addParent(BuildingBlock* parent)
{
    Q_ASSERT(m_parentBb.indexOf(parent) == -1);
    if (m_parentBb.indexOf(parent) == -1) {
        m_parentBb.push_back(parent);
        emit parentAdded(parent);
    }
}

void BuildingBlock::removeParent(Project* project)
{
    Q_ASSERT(m_parentProjects.indexOf(project) != -1);
    int i = m_parentProjects.indexOf(project);
    if (i != -1) {
        m_parentProjects.removeAt(i);
        emit parentRemoved(project);
    }
}

void BuildingBlock::removeParent(BuildingBlock* parent)
{
    Q_ASSERT(m_parentBb.indexOf(parent) != -1);
    int i = m_parentBb.indexOf(parent);
    if (i != -1) {
        m_parentBb.removeAt(i);
        emit parentRemoved(parent);
    }
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
    m_projects.push_back(project);
    emit projectAdded(project);
    return project;
}

void DataModel::deleteProject(Project* project)
{
    const int index = m_projects.indexOf(project);
    Q_ASSERT(index != -1);
    emit projectDeleting(project);
    m_projects.remove(index);
    project->deleteLater();
}

BuildingBlock* DataModel::addBuildingBlock()
{
    auto bb = new BuildingBlock(this);
    m_buildingblocks.push_back(bb);
    emit buildingBlockAdded(bb);
    return bb;
}

bool DataModel::deleteBuildingBlock(BuildingBlock* bb)
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
    emit buildingBlockDeleting(bb);
    m_buildingblocks.removeAt(index);
    bb->deleteLater();
    return true;
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
