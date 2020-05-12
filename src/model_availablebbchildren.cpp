#include "model_availablebbchildren.h"
#include "model_bblist.h"
#include "datamodel.h"

AvailableBuildingBlockChildrenModel::AvailableBuildingBlockChildrenModel(BuildingBlockListModel* model, QObject* parent)
    : QSortFilterProxyModel(parent)
    , m_model(model)
    , m_currentBB(nullptr)
{
    setSourceModel(m_model);
}

void AvailableBuildingBlockChildrenModel::setCurrentBuildingBlock(BuildingBlock* bb)
{
    beginResetModel();
    m_currentBB = bb;
    endResetModel();
}

bool AvailableBuildingBlockChildrenModel::containsCurrent(BuildingBlock* bb) const
{
    QList<BuildingBlock*> checked;
    std::function<bool(BuildingBlock*)> contains = [this, &checked, &contains](BuildingBlock* parent) -> bool {
        if (checked.contains(parent)) {
            return false;
        }
        if (parent->children().contains(m_currentBB)) {
            return true;
        }
        checked.push_back(parent);
        for (auto c : parent->children()) {
            if (contains(c)) {
                return true;
            }
        }
        return false;
    };
    return contains(bb);
}

bool AvailableBuildingBlockChildrenModel::filterAcceptsRow(int sourceRow, const QModelIndex &sourceParent) const
{
    Q_UNUSED(sourceParent)
    if (m_currentBB == nullptr) {
        return false;
    }
    auto bb = m_model->getBuildingBlock(createIndex(sourceRow, 0));
    if (bb == m_currentBB)
        return false;

    return !containsCurrent(bb);
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
        if (m_currentBB->children().contains(childBb)) {
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
        m_currentBB->add(childBb);
    }
    else {
        m_currentBB->remove(childBb);
    }
    emit dataChanged(index, index);
    return true;
}
