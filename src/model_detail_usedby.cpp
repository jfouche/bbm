#include "model_detail_usedby.h"
#include "datamodel.h"

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

BuildingBlockUsedByTreeItem::BuildingBlockUsedByTreeItem(BuildingBlock* bb, const UsedByDataModel& model, TreeItem* parent)
    : TreeItem(parent)
    , m_bb(bb)
    , m_model(model)
{
    auto it = m_model(m_bb);
    if (it != m_model.end()) {
        for (auto parent : it.value().bb) {
            this->appendChild(new BuildingBlockUsedByTreeItem(parent, model, this));
        }
        for (auto parent : it.value().project) {
            this->appendChild(new ProjectUsedByTreeItem(parent, this));
        }
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

// ===========================================================================

RootUsedByTreeItem::RootUsedByTreeItem(Project* project, const UsedByDataModel& model, DetailTreeModel* treeModel)
    : TreeItem(treeModel)
    , m_model(model)
{
    appendChild(new ProjectUsedByTreeItem(project, this));
}

RootUsedByTreeItem::RootUsedByTreeItem(BuildingBlock* bb, const UsedByDataModel& model, DetailTreeModel* treeModel)
    : TreeItem(treeModel)
    , m_model(model)
{
    appendChild(new BuildingBlockUsedByTreeItem(bb, m_model, this));
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
    std::function<void(Project*)> watchProject = [this](Project* project) {
        connect(project, &Project::bbAdded, [this, project](BuildingBlock* bb) {
            m_reverseModel.insert(bb, project);
        });
        connect(project, &Project::bbRemoved, [this, project](BuildingBlock* bb) {
            m_reverseModel.remove(bb, project);
        });
    };

    std::function<void(BuildingBlock*)> watchBb = [this](BuildingBlock* bb) {
        connect(bb, &BuildingBlock::childAdded, [this, bb](BuildingBlock* child) {
            m_reverseModel.insert(child, bb);
        });
        connect(bb, &BuildingBlock::childRemoved, [this, bb](BuildingBlock* child) {
            m_reverseModel.remove(child, bb);
        });
    };

    for (Project* project : m_model->projects()) {
        for (BuildingBlock* bb : project->buildingBlocks()) {
            m_reverseModel.insert(bb, project);
        }
        watchProject(project);
    }

    for (BuildingBlock* bb : m_model->buildingBlocks()) {
        for (BuildingBlock* child : bb->children()) {
            m_reverseModel.insert(child, bb);
        }
        watchBb(bb);
    }
    connect(m_model, &DataModel::projectAdded, watchProject);
    connect(m_model, &DataModel::buildingBlockAdded, watchBb);
}

void UsedByTreeModel::set(Project* project)
{
    beginResetModel();
    delete m_rootItem;
    m_rootItem = new RootUsedByTreeItem(project, m_reverseModel, this);
    endResetModel();
}

void UsedByTreeModel::set(BuildingBlock* bb)
{
    beginResetModel();
    delete m_rootItem;
    m_rootItem = new RootUsedByTreeItem(bb, m_reverseModel, this);
    endResetModel();
}
