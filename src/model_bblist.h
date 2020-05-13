#ifndef BUILDINGBLOCKLISTMODEL_H
#define BUILDINGBLOCKLISTMODEL_H

#include <QAbstractItemModel>

class DataModel;
class BuildingBlock;

class BuildingBlockListModel : public QAbstractListModel
{
    Q_OBJECT

public:
    explicit BuildingBlockListModel(DataModel* datamodel, QObject* parent);

    int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    int columnCount(const QModelIndex& parent = QModelIndex()) const override;
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;
    Qt::ItemFlags flags(const QModelIndex &index) const override;

    BuildingBlock* getBuildingBlock(const QModelIndex &index) const;

private slots:
    void add(BuildingBlock* bb);
    void del(BuildingBlock* bb);

protected:
    DataModel* m_model;
};

#endif // BUILDINGBLOCKLISTMODEL_H
