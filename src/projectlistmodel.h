#ifndef PROJECTLISTMODEL_H
#define PROJECTLISTMODEL_H

#include <QAbstractListModel>
#include "datamodel.h"

class ProjectListModel : public QAbstractListModel
{
    const DataModel* m_model;

    Q_OBJECT

public:
    ProjectListModel(QObject* parent, const DataModel* model);

    int rowCount(const QModelIndex &parent) const override;
    int columnCount(const QModelIndex& parent = QModelIndex()) const override;
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;

private slots:
    void update();
};

#endif // PROJECTLISTMODEL_H
