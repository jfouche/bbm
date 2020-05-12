#ifndef AVAILABLEBUILDINGBLOCKCHILDRENMODEL_H
#define AVAILABLEBUILDINGBLOCKCHILDRENMODEL_H

#include <QSortFilterProxyModel>

class BuildingBlockListModel;
class BuildingBlock;

/**
 * @brief The AvailableBuildingBlockChildrenModel class
 */
class AvailableBuildingBlockChildrenModel : public QSortFilterProxyModel
{
    Q_OBJECT

public:
    explicit AvailableBuildingBlockChildrenModel(BuildingBlockListModel* model, QObject* parent);
    bool filterAcceptsRow(int sourceRow, const QModelIndex &sourceParent) const override;
    Qt::ItemFlags flags(const QModelIndex &index) const override;
    QVariant data(const QModelIndex &index, int role) const override;
    bool setData(const QModelIndex &index, const QVariant &value, int role) override;

    void setCurrentBuildingBlock(BuildingBlock* bb);

private:
    bool containsCurrent(BuildingBlock* bb) const;

private:
    BuildingBlockListModel* m_model;
    BuildingBlock* m_currentBB;
};

#endif // AVAILABLEBUILDINGBLOCKCHILDRENMODEL_H
