#include "model_detail_usedby.h"
#include "datamodel.h"
#include <QDebug>

// ===========================================================================

ProjectUsedByTreeItem::ProjectUsedByTreeItem(Project* project, TreeItem* parent)
    : TreeItem(parent)
    , m_project(project)
{
}

bool ProjectUsedByTreeItem::is(void* dataptr) const
{
    return m_project == dataptr;
}

QVariant ProjectUsedByTreeItem::data(int column) const
{
    switch (column) {
    case COL_NAME: return m_project->name();
    }
    return QVariant();
}

// ===========================================================================

BuildingBlockUsedByTreeItem::BuildingBlockUsedByTreeItem(BuildingBlock* bb, TreeItem* parent)
    : TreeItem(parent)
    , m_bb(bb)
{
    connect(m_bb, qOverload<BuildingBlock*>(&BuildingBlock::parentAdded), this, qOverload<BuildingBlock*>(&BuildingBlockUsedByTreeItem::add));
    connect(m_bb, qOverload<Project*>(&BuildingBlock::parentAdded), this, qOverload<Project*>(&BuildingBlockUsedByTreeItem::add));
    connect(m_bb, qOverload<BuildingBlock*>(&BuildingBlock::parentRemoved), this, qOverload<BuildingBlock*>(&BuildingBlockUsedByTreeItem::remove));
    connect(m_bb, qOverload<Project*>(&BuildingBlock::parentRemoved), this, qOverload<Project*>(&BuildingBlockUsedByTreeItem::remove));
    for (const auto& parent : m_bb->parentBb()) {
        appendChild(new BuildingBlockUsedByTreeItem(parent, this));
    }
    for (const auto& parent : m_bb->parentProject()) {
        appendChild(new ProjectUsedByTreeItem(parent, this));
    }
}

bool BuildingBlockUsedByTreeItem::is(void* dataptr) const
{
    return m_bb == dataptr;
}

QVariant BuildingBlockUsedByTreeItem::data(int column) const
{
    switch (column) {
    case COL_NAME: return m_bb->name();
    case COL_REF: return m_bb->ref();
    case COL_MATURITY: return MaturityLabel[m_bb->maturity()];
    case COL_INFO: return m_bb->info();
    }
    return QVariant();
}

void BuildingBlockUsedByTreeItem::add(Project* parent)
{
    TreeItem::add(new ProjectUsedByTreeItem(parent, this));
}

void BuildingBlockUsedByTreeItem::add(BuildingBlock* parent)
{
    TreeItem::add(new BuildingBlockUsedByTreeItem(parent, this));
}

void BuildingBlockUsedByTreeItem::remove(Project* parent)
{
    remove(parent);
}

void BuildingBlockUsedByTreeItem::remove(BuildingBlock* parent)
{
    remove(parent);
}

// ===========================================================================

RootUsedByTreeItem::RootUsedByTreeItem(Project* project, DetailTreeModel* treeModel)
    : TreeItem(treeModel)
{
    appendChild(new ProjectUsedByTreeItem(project, this));
}

RootUsedByTreeItem::RootUsedByTreeItem(BuildingBlock* bb, DetailTreeModel* treeModel)
    : TreeItem(treeModel)
{
    appendChild(new BuildingBlockUsedByTreeItem(bb, this));
}

bool RootUsedByTreeItem::is(void* dataptr) const
{
    Q_UNUSED(dataptr)
    return false;
}

QVariant RootUsedByTreeItem::data(int column) const
{
    Q_UNUSED(column)
    return QVariant();
}

// ===========================================================================

UsedByTreeModel::UsedByTreeModel(DataModel* model, QObject *parent)
    : DetailTreeModel(model, parent)
{
}

void UsedByTreeModel::set(Project* project)
{
    beginResetModel();
    m_rootItem->deleteLater();
    m_rootItem = new RootUsedByTreeItem(project, this);
    endResetModel();
}

void UsedByTreeModel::set(BuildingBlock* bb)
{
    beginResetModel();
    if (m_rootItem) m_rootItem->deleteLater();
    m_rootItem = new RootUsedByTreeItem(bb, this);
    endResetModel();
}
