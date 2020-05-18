#include "model_detail_usedby.h"
#include "datamodel.h"

BuildingBlockUsedByTreeItem::BuildingBlockUsedByTreeItem(BuildingBlock* bb, TreeItem* parent)
    : TreeItem(parent)
    , m_bb(bb)
{
    connect(bb, &BuildingBlock::childAdded, this, &BuildingBlockUsedByTreeItem::add);
    connect(bb, &BuildingBlock::childRemoved, this, &BuildingBlockUsedByTreeItem::remove);
    connect(bb, &BuildingBlock::changed, this, &BuildingBlockUsedByTreeItem::update);
    for (auto bb : m_bb->children()) {
        appendChild(new BuildingBlockUsedByTreeItem(bb, this));
    }
}

void BuildingBlockUsedByTreeItem::add(BuildingBlock* bb)
{
    TreeItem* childItem = new BuildingBlockUsedByTreeItem(bb, this);
    TreeItem::add(childItem);
}

void BuildingBlockUsedByTreeItem::remove(BuildingBlock* bb)
{
    TreeItem::remove(bb);
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
