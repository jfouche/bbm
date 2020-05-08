#include "model_projectlist.h"
#include "datamodel.h"

ProjectListModel::ProjectListModel(QObject* parent, DataModel* model)
    : QAbstractListModel(parent)
    , m_model(model)
{
}

Project* ProjectListModel::getProject(const QModelIndex &index)
{
    return m_model->projects().at(index.row());
}

int ProjectListModel::rowCount(const QModelIndex &parent) const
{
    Q_ASSERT(parent.parent().isValid() == false);
    return m_model->projects().size();
}

int ProjectListModel::columnCount(const QModelIndex& parent) const
{
    Q_UNUSED(parent)
    return 2;
}

QVariant ProjectListModel::data(const QModelIndex &index, int role) const
{
    if (index.isValid() && (role == Qt::DisplayRole)) {
        const Project* project = m_model->projects().at(index.row());
        Q_ASSERT(project != nullptr);
        switch (index.column()) {
        case 0:
            return project->name();
        case 1:
            return "Col #1";
        }
    }
    return QVariant();
}

