#include "model_projectlist.h"
#include "datamodel.h"

ProjectListModel::ProjectListModel(QObject* parent, DataModel* model)
    : QAbstractListModel(parent)
    , m_model(model)
{
    connect(m_model, &DataModel::cleared, this, &ProjectListModel::clear);
    connect(m_model, &DataModel::projectAdded, this, &ProjectListModel::addProject);
    connect(m_model, &DataModel::projectDeleting, this, &ProjectListModel::delProject);
    for (auto project: m_model->projects()) {
        connect(project, &Project::changed, [this, project]() {
            this->updateProject(project);
        });
    }
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

void ProjectListModel::addProject(Project* project)
{
    Q_ASSERT(m_model->projects().last() == project);
    connect(project, &Project::changed, [this, project]() {
        this->updateProject(project);
    });

    QModelIndex parentIdx;
    const int index = m_model->projects().size() - 1;
    beginInsertRows(parentIdx, index, index);
    endInsertRows();
}

void ProjectListModel::delProject(Project* project)
{
    QModelIndex parentIdx;
    const int index = m_model->projects().indexOf(project);
    beginRemoveRows(parentIdx, index, index);
    endRemoveRows();
}

void ProjectListModel::updateProject(Project* project)
{
    int row = m_model->projects().indexOf(project);
    emit dataChanged(index(row), index(row, columnCount()-1));
}

void ProjectListModel::clear()
{
    beginResetModel();
    endResetModel();
}
