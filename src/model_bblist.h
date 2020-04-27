#ifndef BUILDINGBLOCKLISTMODEL_H
#define BUILDINGBLOCKLISTMODEL_H

#include <QAbstractItemModel>

class DataModel;

class BuildingBlockListModel : public QAbstractListModel
{
    Q_OBJECT

public:
    explicit BuildingBlockListModel(DataModel* datamodel, QObject* parent);

    int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;

private slots:
    void update();

private:
    DataModel* m_datamodel;
};

#endif // BUILDINGBLOCKLISTMODEL_H
