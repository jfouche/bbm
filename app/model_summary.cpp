#include "model_summary.h"
#include "datamodel.h"

#include <QDebug>

SummaryModel::SummaryModel(DataModel* datamodel, QObject* parent)
    : QAbstractListModel(parent)
    , m_model(datamodel)
{
    connect(m_model, &DataModel::cleared, this, &SummaryModel::clear);
    connect(m_model, &DataModel::buildingBlockAdded, this, &SummaryModel::add);
    connect(m_model, &DataModel::buildingBlockDeleting, this, &SummaryModel::del);
    for (auto bb: m_model->buildingBlocks()) {
        connect(bb, &BuildingBlock::changed, [this, bb]() {
            this->update(bb);
        });
    }
}

/*
QVariant SummaryModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    if (orientation == Qt::Horizontal && role == Qt::DisplayRole) {
        switch (section) {
        case COL_NAME : return "Name";
        case COL_REF : return "Ref";
        case COL_MATURITY : return "Maturity";
        case COL_INFO : return "Info";
        }
        qWarning() << "Missing header data";
    }
    return QVariant();
}
*/

int SummaryModel::rowCount(const QModelIndex &parent) const
{
    Q_ASSERT(parent.parent().isValid() == false);
    return m_model->buildingBlocks().size();
}

int SummaryModel::columnCount(const QModelIndex& parent) const
{
    Q_UNUSED(parent)
    return COL_COUNT;
}

QVariant SummaryModel::data(const QModelIndex &index, int role) const
{
    static const QString M2S[] = {"A", "B", "C", "D", "E", "F"};

    if (index.isValid() && (role == Qt::DisplayRole || role == Qt::EditRole)) {
        const BuildingBlock* bb = m_model->buildingBlocks().at(index.row());
        Q_ASSERT(bb != nullptr);
        switch (index.column()) {
        case COL_NAME :     return bb->name();
        case COL_REF :      return bb->ref();
        case COL_MATURITY : return M2S[bb->maturity()];
        case COL_INFO :     return bb->info();
        }
    }
    return QVariant();
}

void SummaryModel::add(BuildingBlock* bb)
{
    Q_ASSERT(m_model->buildingBlocks().last() == bb);
    connect(bb, &BuildingBlock::changed, [this, bb]() {
        this->update(bb);
    });

    QModelIndex parent;
    int index = m_model->buildingBlocks().indexOf(bb);
    beginInsertRows(parent, index, index);
    endInsertRows();
}

void SummaryModel::del(BuildingBlock* bb)
{
    QModelIndex parent;
    int index = m_model->buildingBlocks().indexOf(bb);
    beginRemoveRows(parent, index, index);
}

void SummaryModel::update(BuildingBlock* bb)
{
    int row = m_model->buildingBlocks().indexOf(bb);
    emit dataChanged(index(row), index(row, columnCount()-1));
}

void SummaryModel::clear()
{
    beginResetModel();
    endResetModel();
}
