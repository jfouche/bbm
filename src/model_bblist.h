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

    BuildingBlock* getBuildingBlock(const QModelIndex &index);

private slots:
    void update();

private:
    DataModel* m_model;
};

#endif // BUILDINGBLOCKLISTMODEL_H
