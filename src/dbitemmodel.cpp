#include "dbitemmodel.h"

DbItemModel::DbItemModel(QObject *parent)
    : QAbstractItemModel(parent)
{
}

void DbItemModel::setDatabase(DataModel* base)
{
    m_database = base;
}

QVariant DbItemModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    return QVariant(QString("headerData(%1, %2, %3)").arg(section, orientation, role));
}

QModelIndex DbItemModel::index(int row, int column, const QModelIndex &parent) const
{
    if (!hasIndex(row, column, parent))
        return QModelIndex();

    if(!parent.isValid()    ) {
        return createIndex(row, column, nullptr);
    }

    return createIndex(row, column, 0x00000001);
}

QModelIndex DbItemModel::parent(const QModelIndex &index) const
{
    if (!index.isValid())
        return QModelIndex();

    void* ptr = index.internalPointer();
    if (!ptr) {
        return QModelIndex();
    }
    return createIndex(0, 0, 0x00000001);
}

int DbItemModel::rowCount(const QModelIndex &parent) const
{
    //    if (!parent.isValid())
    //        return 0;

    // FIXME: Implement me!
    return 5;
}

int DbItemModel::columnCount(const QModelIndex &parent) const
{
    if (!parent.isValid())
        return 0;

    // FIXME: Implement me!
    return 2;
}

QVariant DbItemModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid())
        return QVariant();

    // FIXME: Implement me!
    return QVariant("DATA");
}
