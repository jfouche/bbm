#include "model_bblist.h"
#include "datamodel.h"

BuildingBlockListModel::BuildingBlockListModel(DataModel* datamodel, QObject* parent)
    : QAbstractListModel(parent)
    , m_model(datamodel)
{
    connect(m_model, SIGNAL(buildingBlockAdded(BuildingBlock*)), this, SLOT(update()));
    update();
}

void BuildingBlockListModel::update()
{
    beginResetModel();
    endResetModel();
}

BuildingBlock* BuildingBlockListModel::getBuildingBlock(const QModelIndex &index)
{
    return m_model->buildingBlocks().at(index.row());
}

int BuildingBlockListModel::rowCount(const QModelIndex &parent) const
{
    Q_ASSERT(parent.parent().isValid() == false);
    return m_model->buildingBlocks().size();
}

int BuildingBlockListModel::columnCount(const QModelIndex& parent) const
{
    Q_UNUSED(parent)
    return 1;
}

QVariant BuildingBlockListModel::data(const QModelIndex &index, int role) const
{
    if (index.isValid() && (role == Qt::DisplayRole)) {
        auto bb = m_model->buildingBlocks().at(index.row());
        return QString("%1 (%2)").arg(bb->name(), bb->ref());
    }
    return QVariant();
}

Qt::ItemFlags BuildingBlockListModel::flags(const QModelIndex &index) const
{
    auto flags = QAbstractListModel::flags(index);
    if (index.isValid()){
        flags |= Qt::ItemIsUserCheckable;
    }
    return flags;
}
