#include "buildingblocktreemodel.h"
#include "datamodel.h"
#include <QDebug>

enum Columns {
    COL_REF,
    COL_NAME,
    COL_COUNT
};





TreeItem::TreeItem(DataModel* model, BuildingBlockTreeModel* treeModel)
    : m_parent(nullptr)
    , m_treeModel(treeModel)
{
    //connect(model, SIGNAL(buildingBlockAdded(BuildingBlock*)), m_treeModel, SLOT(bbAdded(BuildingBlock*)));
    connect(model, SIGNAL(buildingBlockAdded(BuildingBlock*)), this, SLOT(bbAdded(BuildingBlock*)));
    for (int i = 0; i < model->getBuildingBlockCount(); ++i) {
        BuildingBlock* bb = model->getBuildingBlock(i);
        appendChild(new TreeItem(bb, this, m_treeModel));
    }
}

TreeItem::TreeItem(BuildingBlock* bb, TreeItem* parent, BuildingBlockTreeModel* treeModel)
    : m_parent(parent)
    , m_treeModel(treeModel)
    , m_bb(bb)
{
}

TreeItem::~TreeItem()
{
    qDeleteAll(m_children);
}

void TreeItem::bbAdded(BuildingBlock* bb)
{
    m_treeModel->beginResetModel();
    appendChild(new TreeItem(bb, this, m_treeModel));
    m_treeModel->endResetModel();
}

void TreeItem::appendChild(TreeItem *child)
{
    m_children.append(child);
}

QVariant TreeItem::data(int column) const
{
    switch (column) {
    case COL_REF: return m_bb->ref();
    case COL_NAME: return m_bb->name();
    }
    return QVariant();
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

BuildingBlock* TreeItem::data() const
{
    return m_bb;
}

TreeItem* TreeItem::parentItem()
{
    return m_parent;
}

// ===========================================================================
BuildingBlockTreeModel::BuildingBlockTreeModel(DataModel* model, QObject *parent)
    : QAbstractItemModel(parent)
    , m_model(model)
{
    m_rootItem = new TreeItem(model, this);
}

BuildingBlockTreeModel::~BuildingBlockTreeModel()
{
    delete m_rootItem;
}

void BuildingBlockTreeModel::init()
{

}

void BuildingBlockTreeModel::update()
{
    beginResetModel();
    endResetModel();
}

QModelIndex BuildingBlockTreeModel::index(int row, int column, const QModelIndex &parent) const
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

QModelIndex BuildingBlockTreeModel::parent(const QModelIndex &index) const
{
    if (!index.isValid())
        return QModelIndex();
    TreeItem *childItem = static_cast<TreeItem*>(index.internalPointer());
    TreeItem *parentItem = childItem->parentItem();

    if (parentItem == m_rootItem)
        return QModelIndex();

    return createIndex(parentItem->row(), 0, parentItem);
}

int BuildingBlockTreeModel::rowCount(const QModelIndex &parent) const
{
    TreeItem *parentItem;
    if (parent.column() > 0)
        return 0;

    if (!parent.isValid())
        parentItem = m_rootItem;
    else
        parentItem = static_cast<TreeItem*>(parent.internalPointer());

    return parentItem->childCount();
}

int BuildingBlockTreeModel::columnCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent)
    return COL_COUNT;
}

QVariant BuildingBlockTreeModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid())
        return QVariant();

    if (role != Qt::DisplayRole)
        return QVariant();

    TreeItem *item = static_cast<TreeItem*>(index.internalPointer());
    return item->data(index.column());
}

Qt::ItemFlags BuildingBlockTreeModel::flags(const QModelIndex &index) const
{
    if (!index.isValid())
        return Qt::NoItemFlags;

    return QAbstractItemModel::flags(index);
}

QVariant BuildingBlockTreeModel::headerData(int section, Qt::Orientation orientation,
                                            int role) const
{
    if (orientation == Qt::Horizontal && role == Qt::DisplayRole) {
        switch (section) {
        case COL_REF : return "Reference";
        case COL_NAME : return "Name";
        }
        qWarning() << "Missing header data";
    }
    return QVariant();
}
