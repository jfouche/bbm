#ifndef PROJECTBUILDINGBLOCKLISTMODEL_H
#define PROJECTBUILDINGBLOCKLISTMODEL_H

#include "model_bblist.h"

class Project;

class ProjectBuildingBlockListModel : public BuildingBlockListModel
{
    Q_OBJECT

public:
    explicit ProjectBuildingBlockListModel(DataModel* datamodel, QObject* parent);

    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;
    bool setData(const QModelIndex &index, const QVariant &value, int role) override;
    Qt::ItemFlags flags(const QModelIndex &index) const override;

    void setProject(Project* project);

private:
    Project* m_project;
};

#endif // PROJECTBUILDINGBLOCKLISTMODEL_H
