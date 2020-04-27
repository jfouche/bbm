#include "model_bblist.h"
#include "datamodel.h"

BuildingBlockListModel::BuildingBlockListModel(DataModel* datamodel, QObject* parent)
    : QAbstractListModel(parent)
    , m_datamodel(datamodel)
{
    connect(m_datamodel, SIGNAL(buildingBlockAdded(BuildingBlock*)), this, SLOT(update()));
    beginResetModel();
    endResetModel();
}

void BuildingBlockListModel::update()
{
    beginResetModel();
    endResetModel();
}

int BuildingBlockListModel::rowCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent)
//    qDebug() << "BuildingBlockListModel::rowCount() => " << m_datamodel->buildingBlocks().size();;
    return m_datamodel->buildingBlocks().size();
}

QVariant BuildingBlockListModel::data(const QModelIndex &index, int role) const
{
    Q_UNUSED(role)
    auto bb = m_datamodel->buildingBlocks().at(index.row());
//    qDebug() << "BuildingBlockListModel::data(" << index.row() << ") => " << QString("%1 (%2)").arg(bb->name(), bb->ref());
    return QString("%1 (%2)").arg(bb->name(), bb->ref());
}
