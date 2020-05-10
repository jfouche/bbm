#include "model_availablebbchildren.h"
#include "model_bblist.h"
#include "datamodel.h"

AvailableBuildingBlockChildrenModel::AvailableBuildingBlockChildrenModel(BuildingBlockListModel* model, QObject* parent)
    : QSortFilterProxyModel(parent)
    , m_model(model)
    , m_parentBB(nullptr)
{
    setSourceModel(m_model);
}

void AvailableBuildingBlockChildrenModel::setParentBuildingBlock(BuildingBlock* parentBb)
{
    beginResetModel();
    m_parentBB = parentBb;
    endResetModel();
}

bool AvailableBuildingBlockChildrenModel::filterAcceptsRow(int sourceRow, const QModelIndex &sourceParent) const
{
    Q_UNUSED(sourceParent)
    if (m_parentBB == nullptr) {
        return false;
    }
    auto bb = m_model->getBuildingBlock(createIndex(sourceRow, 0));
    if (bb == m_parentBB)
        return false;
    if (bb->children().contains(m_parentBB)) {
        return false;
    }
    return true;
}

Qt::ItemFlags AvailableBuildingBlockChildrenModel::flags(const QModelIndex &index) const
{
    auto flags = QSortFilterProxyModel::flags(index);
    if (index.isValid()) {
        flags |= Qt::ItemIsUserCheckable;
    }
    return flags;
}

QVariant AvailableBuildingBlockChildrenModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid())
        return QVariant();

    auto srcIndex = mapToSource(index);
    auto childBb = m_model->getBuildingBlock(srcIndex);

    if (role == Qt::CheckStateRole) {
        if (m_parentBB->children().contains(childBb)) {
            return Qt::Checked;
        }
        else {
            return Qt::Unchecked;
        }
    }

    return QSortFilterProxyModel::data(index, role);
}

bool AvailableBuildingBlockChildrenModel::setData(const QModelIndex &index, const QVariant &value, int role)
{
    if(!index.isValid() || role != Qt::CheckStateRole)
            return false;

    auto srcIndex = mapToSource(index);
    auto childBb = m_model->getBuildingBlock(srcIndex);

    if(value == Qt::Checked) {
        m_parentBB->add(childBb);
    }
    else {
        m_parentBB->remove(childBb);
    }
    emit dataChanged(index, index);
    return true;
}
