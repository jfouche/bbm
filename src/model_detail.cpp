#include "datamodel.h"
#include "model_detail.h"
#include <QColor>
#include <QDebug>

enum Columns {
    COL_NAME,
    COL_REF,
    COL_MATURITY,
    COL_INFO,
    COL_COUNT
};

static const std::vector<QString> MaturityLabel = {"A", "B", "C", "D", "E", "F"};

// ===========================================================================

TreeItem::TreeItem(DetailTreeModel* treeModel)
    : m_parent(nullptr)
    , m_treeModel(treeModel)
{
}

TreeItem::TreeItem(TreeItem* parent)
    : m_parent(parent)
    , m_treeModel(parent->m_treeModel)
{
}

TreeItem::~TreeItem()
{
    qDeleteAll(m_children);
}

QModelIndex TreeItem::index(int col)
{
    return m_treeModel->createIndex(row(), col, this);
}

void TreeItem::add(BuildingBlock* bb)
{
    QModelIndex parentIdx = index();
    const int first = m_children.size();
    m_treeModel->beginInsertRows(parentIdx, first, first);
    appendChild(new BuildingBlockTreeItem(bb, this));
    m_treeModel->endInsertRows();
}

void TreeItem::remove(BuildingBlock* bb)
{
    QModelIndex parentIdx = index();
    int pos = 0;
    for (auto it = m_children.begin(); it != m_children.end(); /* no it++ */) {
        const TreeItem* item = *it;
        if (item->is(bb)) {
            m_treeModel->beginRemoveRows(parentIdx, pos, pos);
            it = m_children.erase(it);
            delete item;
            m_treeModel->endRemoveRows();
        }
        else {
            ++it;
        }
        pos++;
    }
}

void TreeItem::update()
{
    QModelIndex topLeft = index(0);
    QModelIndex bottomRight = index(m_treeModel->columnCount());
    m_treeModel->dataChanged(topLeft, bottomRight);
}


void TreeItem::appendChild(TreeItem *child)
{
    m_children.append(child);
}

TreeItem* TreeItem::child(int row)
{
    if (row < 0 || row >= m_children.size())
        return nullptr;
    return m_children.at(row);
}

int TreeItem::childCount() const
{
    return m_children.count();
}

int TreeItem::row() const
{
    if (!m_parent) {
        return 0;
    }
    return m_parent->m_children.indexOf(const_cast<TreeItem*>(this));
}

TreeItem* TreeItem::parentItem()
{
    return m_parent;
}

// ===========================================================================

ProjectTreeItem::ProjectTreeItem(Project* project, TreeItem* parent)
    : TreeItem(parent)
    , m_project(project)
{
    connect(project, &Project::bbAdded, this, &ProjectTreeItem::add);
    connect(project, &Project::bbRemoved, this, &ProjectTreeItem::remove);
    connect(project, &Project::changed, this, &ProjectTreeItem::update);
    for (auto bb : m_project->buildingBlocks()) {
        appendChild(new BuildingBlockTreeItem(bb, this));
    }
}

bool ProjectTreeItem::is(void* dataptr) const
{
    return m_project == dataptr;
}

QVariant ProjectTreeItem::data(int column) const
{
    switch (column) {
    case COL_NAME: return m_project->name();
    }
    return QVariant();
}

// ===========================================================================

BuildingBlockTreeItem::BuildingBlockTreeItem(BuildingBlock* bb, TreeItem* parent)
    : TreeItem(parent)
    , m_bb(bb)
{
    connect(bb, &BuildingBlock::childAdded, this, &BuildingBlockTreeItem::add);
    connect(bb, &BuildingBlock::childRemoved, this, &BuildingBlockTreeItem::remove);
    connect(bb, &BuildingBlock::changed, this, &BuildingBlockTreeItem::update);
    for (auto bb : m_bb->children()) {
        appendChild(new BuildingBlockTreeItem(bb, this));
    }
}
bool BuildingBlockTreeItem::is(void* dataptr) const
{
    return m_bb == dataptr;
}

QVariant BuildingBlockTreeItem::data(int column) const
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

RootTreeItem::RootTreeItem(Project* project, DetailTreeModel* treeModel)
    : TreeItem(treeModel)
{
    appendChild(new ProjectTreeItem(project, this));
}

RootTreeItem::RootTreeItem(BuildingBlock* bb, DetailTreeModel* treeModel)
    : TreeItem(treeModel)
{
    appendChild(new BuildingBlockTreeItem(bb, this));
}

bool RootTreeItem::is(void* dataptr) const
{
    Q_UNUSED(dataptr)
    return false;
}

QVariant RootTreeItem::data(int column) const
{
    Q_UNUSED(column)
    return QVariant();
}

// ===========================================================================

DetailTreeModel::DetailTreeModel(DataModel* model, QObject *parent)
    : QAbstractItemModel(parent)
    , m_model(model)
    , m_rootItem(nullptr)
{
}

DetailTreeModel::~DetailTreeModel()
{
    delete m_rootItem;
}

QVariant DetailTreeModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    if (orientation == Qt::Horizontal && role == Qt::DisplayRole) {
        switch (section) {
        case COL_NAME : return "Name";
        case COL_REF : return "Ref";
        case COL_MATURITY : return "Maturity";
        case COL_INFO : return "Info";
        }
        qWarning() << "Missing header data";
    }
    return QVariant();
}

QModelIndex DetailTreeModel::index(int row, int column, const QModelIndex &parent) const
{
    if (!hasIndex(row, column, parent))
        return QModelIndex();

    TreeItem *parentItem;

    if (!parent.isValid())
        parentItem = m_rootItem;
    else
        parentItem = static_cast<TreeItem*>(parent.internalPointer());

    TreeItem *childItem = parentItem->child(row);
    if (childItem)
        return createIndex(row, column, childItem);
    return QModelIndex();
}

QModelIndex DetailTreeModel::parent(const QModelIndex &index) const
{
    if (!index.isValid())
        return QModelIndex();
    TreeItem *childItem = static_cast<TreeItem*>(index.internalPointer());
    TreeItem *parentItem = childItem->parentItem();

    if (parentItem == m_rootItem)
        return QModelIndex();

    return createIndex(parentItem->row(), 0, parentItem);
}

int DetailTreeModel::rowCount(const QModelIndex &parent) const
{
    if (!m_rootItem) {
        return 0;
    }

    if (parent.column() > 0)
        return 0;

    TreeItem *parentItem = m_rootItem;
    if (parent.isValid()) {
        parentItem = static_cast<TreeItem*>(parent.internalPointer());
    }
    return parentItem->childCount();
}

int DetailTreeModel::columnCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent)
    return COL_COUNT;
}

QVariant DetailTreeModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid())
        return QVariant();

    TreeItem *item = treeItem(index);
    if (role == Qt::DisplayRole) {
        return item->data(index.column());
    }
    else if (role == Qt::ForegroundRole) {
        // A child BB should be gray
        TreeItem* parent = item->parentItem();
        if (parent && parent != m_rootItem) {
            return QColor(Qt::darkGray);
        }
    }
    return QVariant();
}

TreeItem* DetailTreeModel::treeItem(const QModelIndex& index) const
{
    return static_cast<TreeItem*>(index.internalPointer());
}

TreeItem* DetailTreeModel::treeItem(int row, const QModelIndex &parent)
{
    TreeItem* item = m_rootItem;
    if (parent.isValid()) {
        item = treeItem(parent);
    }
    return item->child(row);
}

void DetailTreeModel::set(Project* project)
{
    beginResetModel();
    delete m_rootItem;
    m_rootItem = new RootTreeItem(project, this);
    endResetModel();
}

void DetailTreeModel::set(BuildingBlock* bb)
{
    beginResetModel();
    delete m_rootItem;
    m_rootItem = new RootTreeItem(bb, this);
    endResetModel();
}
