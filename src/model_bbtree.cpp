#include "model_bbtree.h"
#include "datamodel.h"
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

TreeItem::TreeItem(DataModel* model, BuildingBlockTreeModel* treeModel)
    : m_parent(nullptr)
    , m_treeModel(treeModel)
    , m_bb(nullptr)
{
    connect(model, SIGNAL(buildingBlockAdded(BuildingBlock*)), this, SLOT(add(BuildingBlock*)));
    for (BuildingBlock* bb : model->buildingBlocks()) {
        appendChild(new TreeItem(bb, this, m_treeModel));
    }
}

TreeItem::TreeItem(BuildingBlock* bb, TreeItem* parent, BuildingBlockTreeModel* treeModel)
    : m_parent(parent)
    , m_treeModel(treeModel)
    , m_bb(bb)
{
    connect(bb, SIGNAL(childAdded(BuildingBlock*, BuildingBlock*)), this, SLOT(add(BuildingBlock*, BuildingBlock*)));
    connect(bb, SIGNAL(childRemoved(BuildingBlock*, BuildingBlock*)), this, SLOT(remove(BuildingBlock*, BuildingBlock*)));
    for (auto bb : m_bb->children()) {
        appendChild(new TreeItem(bb, this, m_treeModel));
    }
}

TreeItem::~TreeItem()
{
    qDeleteAll(m_children);
}

void TreeItem::add(BuildingBlock* bb)
{
    QModelIndex parentIdx = QModelIndex();
    if (m_bb) {
        parentIdx = m_treeModel->createIndex(row(), 0, this);
    }
    const int first = m_children.size();
    m_treeModel->beginInsertRows(parentIdx, first, first);
    appendChild(new TreeItem(bb, this, m_treeModel));
    m_treeModel->endInsertRows();
}

void TreeItem::add(BuildingBlock* bb, BuildingBlock* parent)
{
    Q_ASSERT(data() == parent);
    add(bb);
}

void TreeItem::remove(BuildingBlock* bb, BuildingBlock* parent)
{
    Q_ASSERT(data() == parent);

    int pos = 0;
    for (auto it = m_children.begin(); it != m_children.end(); /* no it++ */) {
        const TreeItem* item = *it;
        if (item->m_bb == bb) {
            const auto parentIdx = m_treeModel->createIndex(row(), 0, this);
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

void TreeItem::appendChild(TreeItem *child)
{
    m_children.append(child);
}

QVariant TreeItem::data(int column) const
{
    switch (column) {
    case COL_REF: return m_bb->ref();
    case COL_NAME: return m_bb->name();
    case COL_MATURITY: return MaturityLabel[m_bb->maturity()];
    case COL_INFO: return m_bb->info();
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

    TreeItem *item = treeItem(index);
    if (role == Qt::DisplayRole) {
        return item->data(index.column());
    }
    else if (role == Qt::ForegroundRole) {
        // A child BB should be gray
        TreeItem* parent = item->parentItem();
        if (parent && parent->data()) {
            return QColor(Qt::darkGray);
        }
    }
    return QVariant();
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
        case COL_INFO : return "Info";
        case COL_MATURITY : return "Maturity";
        }
        qWarning() << "Missing header data";
    }
    return QVariant();
}

TreeItem* BuildingBlockTreeModel::treeItem(const QModelIndex& index) const
{
    return static_cast<TreeItem*>(index.internalPointer());
}

TreeItem* BuildingBlockTreeModel::treeItem(int row, const QModelIndex &parent)
{
    TreeItem* item = m_rootItem;
    if (parent.isValid()) {
        item = treeItem(parent);
    }
    return item->child(row);
}
