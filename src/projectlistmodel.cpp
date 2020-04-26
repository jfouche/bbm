#include "projectlistmodel.h"
#include <QDebug>

ProjectListModel::ProjectListModel(QObject* parent, DataModel* model)
    : QAbstractListModel(parent)
    , m_model(model)
{
    qDebug() << "ProjectListModel::ProjectListModel()";
    connect(model, SIGNAL(dbChanged()), this, SLOT(update()));
    beginResetModel();
    endResetModel();
}

Project* ProjectListModel::getProject(const QModelIndex &index)
{
    return m_model->projects().at(index.row());
}

int ProjectListModel::rowCount(const QModelIndex &parent) const
{
    qDebug() << "ProjectListModel::rowCount()";
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
    const Project* project = m_model->projects().at(index.row());
    Q_ASSERT(project != nullptr);
    qDebug() << "ProjectListModel::data(" << index.row() << ") : " << project->name();
    if (!index.isValid())
        return QVariant();
    if ( role == Qt::DisplayRole) {
        switch (index.column()) {
        case 0:
            return project->name();
        case 1:
            return "Col #1";
        }
    }
    return QVariant();
}

void ProjectListModel::update()
{
    beginResetModel();
    endResetModel();
}
