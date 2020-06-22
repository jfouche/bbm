#include "datamodel.h"
#include "model_detail.h"
#include <QColor>
#include <QDebug>


const std::vector<QString> TreeItem::MaturityLabel = {"A", "B", "C", "D", "E", "F"};

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

const QList<TreeItem*>& TreeItem::children() const
{
    return m_children;
}

void TreeItem::add(TreeItem* item)
{
    int pos = m_children.size();
    appendChild(item);
    m_treeModel->childAdded(item, pos);
}

void TreeItem::remove(void* data)
{
    int pos = 0;
    for (auto it = m_children.begin(); it != m_children.end(); /* no it++ */) {
        TreeItem* item = *it;
        if (item->is(data)) {
            it = m_children.erase(it);
            m_treeModel->childRemoved(item, pos);
            delete item;
        }
        else {
            ++it;
        }
        pos++;
    }
}

void TreeItem::update()
{
    m_treeModel->update(this);
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


DetailTreeModel::DetailTreeModel(DataModel* model, QObject *parent)
    : QAbstractItemModel(parent)
    , m_model(model)
    , m_rootItem(nullptr)
{
    connect(m_model, &DataModel::cleared, this, &DetailTreeModel::clear);
}

DetailTreeModel::~DetailTreeModel()
{
}

QVariant DetailTreeModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    if (orientation == Qt::Horizontal && role == Qt::DisplayRole) {
        switch (section) {
        case TreeItem::COL_NAME : return "Name";
        case TreeItem::COL_REF : return "Ref";
        case TreeItem::COL_MATURITY : return "Maturity";
        case TreeItem::COL_INFO : return "Info";
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
    return TreeItem::COL_COUNT;
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

QModelIndex DetailTreeModel::index(TreeItem* item, int col) const
{
    return createIndex(item->row(), col, item);
}

void DetailTreeModel::childAdded(TreeItem* item, int pos)
{
    QModelIndex parentIdx = index(item->parentItem());
    beginInsertRows(parentIdx, pos, pos);
    endInsertRows();
}

void DetailTreeModel::childRemoved(TreeItem* item, int pos)
{
    QModelIndex parentIdx = index(item->parentItem());
    beginRemoveRows(parentIdx, pos, pos);
    endRemoveRows();
}

void DetailTreeModel::update(TreeItem* item)
{
    emit dataChanged(index(item), index(item, columnCount()-1));
}

void DetailTreeModel::clear()
{
    beginResetModel();
    if (m_rootItem) {
        m_rootItem->deleteLater();
    }
    m_rootItem = nullptr;
    endResetModel();
}