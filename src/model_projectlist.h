#ifndef PROJECTLISTMODEL_H
#define PROJECTLISTMODEL_H

#include <QAbstractListModel>

class DataModel;
class Project;

class ProjectListModel : public QAbstractListModel
{
    Q_OBJECT

public:
    ProjectListModel(QObject* parent, DataModel* model);

    int rowCount(const QModelIndex &parent) const override;
    int columnCount(const QModelIndex& parent = QModelIndex()) const override;
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;

    Project* getProject(const QModelIndex &index);

private slots:

private:
    DataModel* m_model;
};

#endif // PROJECTLISTMODEL_H
