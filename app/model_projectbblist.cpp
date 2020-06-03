#include "model_projectbblist.h"
#include "datamodel.h"

ProjectBuildingBlockListModel::ProjectBuildingBlockListModel(DataModel* datamodel, QObject* parent)
    : BuildingBlockListModel(datamodel, parent)
    , m_project(nullptr)
{
}

void ProjectBuildingBlockListModel::setProject(Project* project)
{
    beginResetModel();
    m_project = project;
    endResetModel();
}

Qt::ItemFlags ProjectBuildingBlockListModel::flags(const QModelIndex &index) const
{
    auto flags = BuildingBlockListModel::flags(index);
    if (index.isValid()) {
        flags |= Qt::ItemIsUserCheckable;
    }
    return flags;
}

QVariant ProjectBuildingBlockListModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid() || !m_project)
        return QVariant();

    if (role == Qt::CheckStateRole) {
        auto bb = getBuildingBlock(index);
        if (m_project->contains(bb)) {
            return Qt::Checked;
        }
        else {
            return Qt::Unchecked;
        }
    }
    return BuildingBlockListModel::data(index, role);
}

bool ProjectBuildingBlockListModel::setData(const QModelIndex &index, const QVariant &value, int role)
{
    Q_ASSERT(m_project);

    if(!index.isValid())
        return false;

    if (role == Qt::CheckStateRole) {
        auto bb = getBuildingBlock(index);
        if(value == Qt::Checked) {
            m_project->add(bb);
        }
        else {
            m_project->remove(bb);
        }
        // We should observe bbAdded/bbRemoved signals, but it's easier this way
        emit dataChanged(index, index);
    }
    return true;
}

