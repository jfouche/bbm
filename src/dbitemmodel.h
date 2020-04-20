#ifndef DBITEMMODEL_H
#define DBITEMMODEL_H

#include <QAbstractItemModel>
#include "datamodel.h"

class DbItemModel : public QAbstractItemModel
{
    DataModel* m_database;

    Q_OBJECT

public:
    explicit DbItemModel(QObject *parent = nullptr);

    void setDatabase(DataModel* base);

    // Header:
    QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const override;

    // Basic functionality:
    QModelIndex index(int row, int column,
                      const QModelIndex &parent = QModelIndex()) const override;
    QModelIndex parent(const QModelIndex &index) const override;

    int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    int columnCount(const QModelIndex &parent = QModelIndex()) const override;

    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;

private:
};

#endif // DBITEMMODEL_H
