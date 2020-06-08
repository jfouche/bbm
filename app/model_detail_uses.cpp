#include "model_detail_uses.h"
#include "datamodel.h"

ProjectUsesTreeItem::ProjectUsesTreeItem(Project* project, TreeItem* parent)
    : TreeItem(parent)
    , m_project(project)
{
    connect(project, &Project::bbAdded, this, &ProjectUsesTreeItem::add);
    connect(project, &Project::bbRemoved, this, &ProjectUsesTreeItem::remove);
    connect(project, &Project::changed, this, &ProjectUsesTreeItem::update);
    for (auto bb : m_project->buildingBlocks()) {
        appendChild(new BuildingBlockUsesTreeItem(bb, this));
    }
}

void ProjectUsesTreeItem::add(BuildingBlock* bb)
{
    TreeItem::add(new BuildingBlockUsesTreeItem(bb, this));
}

void ProjectUsesTreeItem::remove(BuildingBlock* bb)
{
    TreeItem::remove(bb);
}

bool ProjectUsesTreeItem::is(void* dataptr) const
{
    return m_project == dataptr;
}

QVariant ProjectUsesTreeItem::data(int column) const
{
    switch (column) {
    case COL_NAME: return m_project->name();
    }
    return QVariant();
}

// ===========================================================================

BuildingBlockUsesTreeItem::BuildingBlockUsesTreeItem(BuildingBlock* bb, TreeItem* parent)
    : TreeItem(parent)
    , m_bb(bb)
{
    connect(bb, &BuildingBlock::childAdded, this, &BuildingBlockUsesTreeItem::add);
    connect(bb, &BuildingBlock::childRemoved, this, &BuildingBlockUsesTreeItem::remove);
    connect(bb, &BuildingBlock::changed, this, &BuildingBlockUsesTreeItem::update);
    for (auto bb : m_bb->children()) {
        appendChild(new BuildingBlockUsesTreeItem(bb, this));
    }
}

void BuildingBlockUsesTreeItem::add(BuildingBlock* bb)
{
    TreeItem::add(new BuildingBlockUsesTreeItem(bb, this));
}

void BuildingBlockUsesTreeItem::remove(BuildingBlock* bb)
{
    TreeItem::remove(bb);
}

bool BuildingBlockUsesTreeItem::is(void* dataptr) const
{
    return m_bb == dataptr;
}

QVariant BuildingBlockUsesTreeItem::data(int column) const
{
    switch (column) {
    case COL_NAME: return m_bb->name();
    case COL_REF: return m_bb->ref();
    case COL_MATURITY: return MaturityLabel[m_bb->maturity()];
    case COL_INFO: return m_bb->info();
    }
    return QVariant();
}

// ===========================================================================

RootUsesTreeItem::RootUsesTreeItem(Project* project, DetailTreeModel* treeModel)
    : TreeItem(treeModel)
{
    appendChild(new ProjectUsesTreeItem(project, this));
}

RootUsesTreeItem::RootUsesTreeItem(BuildingBlock* bb, DetailTreeModel* treeModel)
    : TreeItem(treeModel)
{
    appendChild(new BuildingBlockUsesTreeItem(bb, this));
}

bool RootUsesTreeItem::is(void* dataptr) const
{
    Q_UNUSED(dataptr)
    return false;
}

QVariant RootUsesTreeItem::data(int column) const
{
    Q_UNUSED(column)
    return QVariant();
}

// ===========================================================================

UsesTreeModel::UsesTreeModel(DataModel* model, QObject *parent)
    : DetailTreeModel(model, parent)
{
}

void UsesTreeModel::set(Project* project)
{
    beginResetModel();
    if (m_rootItem) m_rootItem->deleteLater();
    m_rootItem = new RootUsesTreeItem(project, this);
    endResetModel();
}

void UsesTreeModel::set(BuildingBlock* bb)
{
    beginResetModel();
    if (m_rootItem) m_rootItem->deleteLater();
    m_rootItem = new RootUsesTreeItem(bb, this);
    endResetModel();
}
