#include "model_detail_usedby.h"
#include "datamodel.h"

UsedByBuildingBlock::UsedByBuildingBlock(BuildingBlock* bb)
    : m_bb(bb)
{
}

const BuildingBlock* UsedByBuildingBlock::bb() const
{
    return m_bb;
}

const UsedByParents& UsedByBuildingBlock::parents() const
{
    return m_parents;
}

void UsedByBuildingBlock::addBb(UsedByBuildingBlock* parent)
{
    m_parents.bb.insert(parent);
    emit bbAdded(parent);
}

void UsedByBuildingBlock::addProject(Project* parent)
{
    m_parents.project.insert(parent);
    emit projectAdded(parent);
}

void UsedByBuildingBlock::removeBb(UsedByBuildingBlock* parent)
{
    m_parents.bb.remove(parent);
    emit bbRemoved(parent);
}

void UsedByBuildingBlock::removeProject(Project* parent)
{
    m_parents.project.remove(parent);
    emit projectRemoved(parent);
}

// ===========================================================================

UsedByDataModel::UsedByDataModel(DataModel* model)
{
    std::function<void(Project*)> watchProject = [this](Project* project) {
        connect(project, &Project::bbAdded, [this, project](BuildingBlock* bb) {
            insert(bb, project);
        });
        connect(project, &Project::bbRemoved, [this, project](BuildingBlock* bb) {
            remove(bb, project);
        });
    };

    std::function<void(BuildingBlock*)> watchBb = [this](BuildingBlock* bb) {
        connect(bb, &BuildingBlock::childAdded, [this, bb](BuildingBlock* child) {
            insert(child, bb);
        });
        connect(bb, &BuildingBlock::childRemoved, [this, bb](BuildingBlock* child) {
            remove(child, bb);
        });
    };

    for (Project* project : model->projects()) {
        for (BuildingBlock* bb : project->buildingBlocks()) {
            insert(bb, project);
        }
        watchProject(project);
    }

    for (BuildingBlock* bb : model->buildingBlocks()) {
        for (BuildingBlock* child : bb->children()) {
            insert(child, bb);
        }
        watchBb(bb);
    }
    connect(model, &DataModel::projectAdded, watchProject);
    connect(model, &DataModel::buildingBlockAdded, watchBb);
}

UsedByBuildingBlock* UsedByDataModel::get(BuildingBlock* bb) {
    auto it = std::find_if(m_listBb.begin(), m_listBb.end(), [bb](UsedByBuildingBlock* v) {
        return v->bb() == bb;
    });

    if (it != m_listBb.end()) {
        return *it;
    }
    UsedByBuildingBlock* ubBb = new UsedByBuildingBlock(bb);
    m_listBb.append(ubBb);
    return ubBb;
}

void UsedByDataModel::insert(BuildingBlock* bb, BuildingBlock* parent) {
    get(bb)->addBb(get(parent));
}

void UsedByDataModel::insert(BuildingBlock* bb, Project* parent) {
    get(bb)->addProject(parent);
}

void UsedByDataModel::remove(BuildingBlock* bb, BuildingBlock* parent) {
    get(bb)->removeBb(get(parent));
}

void UsedByDataModel::remove(BuildingBlock* bb, Project* parent) {
    get(bb)->removeProject(parent);
}


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

BuildingBlockUsedByTreeItem::BuildingBlockUsedByTreeItem(UsedByBuildingBlock* bb, TreeItem* parent)
    : TreeItem(parent)
    , m_bb(bb)
{
    connect(bb, &UsedByBuildingBlock::bbAdded, this, &BuildingBlockUsedByTreeItem::addBb);
    connect(bb, &UsedByBuildingBlock::bbRemoved, this, &BuildingBlockUsedByTreeItem::removeBb);
    for (const auto& parent : bb->parents().bb) {
        appendChild(new BuildingBlockUsedByTreeItem(parent, this));
    }
    for (const auto& parent : bb->parents().project) {
        appendChild(new ProjectUsedByTreeItem(parent, this));
    }
}

void BuildingBlockUsedByTreeItem::addBb(UsedByBuildingBlock* bb)
{
    TreeItem::add(new BuildingBlockUsedByTreeItem(bb, this));
}

void BuildingBlockUsedByTreeItem::addProject(Project* project)
{
    TreeItem::add(new ProjectUsedByTreeItem(project, this));
}

void BuildingBlockUsedByTreeItem::removeBb(UsedByBuildingBlock* bb)
{
    TreeItem::remove(bb);
}

void BuildingBlockUsedByTreeItem::removeProject(Project* project)
{
    TreeItem::remove(project);
}

bool BuildingBlockUsedByTreeItem::is(void* dataptr) const
{
    return m_bb == dataptr;
}

QVariant BuildingBlockUsedByTreeItem::data(int column) const
{
    switch (column) {
    case COL_NAME: return m_bb->bb()->name();
    case COL_REF: return m_bb->bb()->ref();
    case COL_MATURITY: return MaturityLabel[m_bb->bb()->maturity()];
    case COL_INFO: return m_bb->bb()->info();
    }
    return QVariant();
}

// ===========================================================================

RootUsedByTreeItem::RootUsedByTreeItem(Project* project, DetailTreeModel* treeModel)
    : TreeItem(treeModel)
{
    appendChild(new ProjectUsedByTreeItem(project, this));
}

RootUsedByTreeItem::RootUsedByTreeItem(UsedByBuildingBlock* bb, DetailTreeModel* treeModel)
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
    , m_reverseModel(model)
{
}

void UsedByTreeModel::set(Project* project)
{
    beginResetModel();
    delete m_rootItem;
    m_rootItem = new RootUsedByTreeItem(project, this);
    endResetModel();
}

void UsedByTreeModel::set(BuildingBlock* bb)
{
    beginResetModel();
    delete m_rootItem;
    m_rootItem = new RootUsedByTreeItem(m_reverseModel.get(bb), this);
    endResetModel();
}
