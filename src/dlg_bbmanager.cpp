#include "dlg_bbmanager.h"
#include "ui_dlg_bbmanager.h"
#include "datamodel.h"
#include "model_bblist.h"

#include <QCompleter>

#include <QDebug>


// ===========================================================================

BuildingBlocksCompleterModel::BuildingBlocksCompleterModel(DataModel* datamodel, QObject* parent)
    : QAbstractListModel(parent)
    , m_model(datamodel)
{
}

void BuildingBlocksCompleterModel::filter(const QString& filter)
{
    qDebug() << "BuildingBlocksCompleterModel::filter(" << filter << ")";
    beginResetModel();
    m_filter = filter;
    endResetModel();
}

const QString& BuildingBlocksCompleterModel::filter() const
{
    return m_filter;
}

int BuildingBlocksCompleterModel::rowCount(const QModelIndex &parent) const
{
    Q_ASSERT(parent.parent().isValid() == false);
    return m_model->buildingBlocks().size();
}

int BuildingBlocksCompleterModel::columnCount(const QModelIndex& parent) const
{
    Q_UNUSED(parent)
    return 1;
}

QVariant BuildingBlocksCompleterModel::data(const QModelIndex &index, int role) const
{
    if (index.isValid() && (role == Qt::DisplayRole || role == Qt::EditRole)) {
        auto bb = m_model->buildingBlocks().at(index.row());
        return QString("%1 (%2)").arg(bb->name(), bb->ref());
    }
    return QVariant();
}

// ===========================================================================

AvailableBuildingBlockChildrenModel::AvailableBuildingBlockChildrenModel(DataModel* datamodel, QObject* parent)
    : QSortFilterProxyModel(parent)
    , m_model(new BuildingBlockListModel(datamodel, this))
    , m_parentBB(nullptr)
{
    setSourceModel(m_model);
}

void AvailableBuildingBlockChildrenModel::setParentBuildingBlock(const BuildingBlock* parentBb)
{
    beginResetModel();
    m_parentBB = parentBb;
    endResetModel();
}

bool AvailableBuildingBlockChildrenModel::filterAcceptsRow(int sourceRow, const QModelIndex &sourceParent) const
{
    if (m_model->getBuildingBlock(sourceParent) == m_parentBB)
        return false;
    return true;
}

Qt::ItemFlags AvailableBuildingBlockChildrenModel::flags(const QModelIndex &index) const
{
    if (!index.isValid()) {
        return Qt::NoItemFlags;
    }
    auto flags = QSortFilterProxyModel::flags(index);
    flags |= Qt::ItemIsUserCheckable;
    return flags;
}

// ===========================================================================

BuildingBlocksFilterModel::BuildingBlocksFilterModel(QObject* parent)
    : QSortFilterProxyModel(parent)
{
}

bool BuildingBlocksFilterModel::setData(const QModelIndex &index, const QVariant &value, int role)
{
    bool ret = QSortFilterProxyModel::setData(index, value);
    return ret;
}

bool BuildingBlocksFilterModel::setItemData(const QModelIndex &index, const QMap<int, QVariant> &roles)
{
    bool ret = QSortFilterProxyModel::setItemData(index, roles);
    return ret;
}

void BuildingBlocksFilterModel::filter(const QString& filter)
{
    beginResetModel();
    m_filter = filter;
    endResetModel();
}

bool BuildingBlocksFilterModel::filterAcceptsRow(int source_row, const QModelIndex &source_parent) const
{
    if (m_filter.isEmpty()) return true;
    auto srcModel = sourceModel();
    auto idx = srcModel->index(source_row, 0, source_parent);
    return srcModel->data(idx).toString().contains(m_filter);
}

// ===========================================================================

BuildingBlockMgrDlg::BuildingBlockMgrDlg(DataModel* model, QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::BuildingBlockMgrDlg)
    , m_model(model)
{
    ui->setupUi(this);

    m_listBbModel = new BuildingBlockListModel(m_model, this);
    ui->listBuildingBlocksChildren->setModel(m_listBbModel);

    m_BbCompleterModel = new BuildingBlocksCompleterModel(m_model, this);
    QCompleter* completer = new QCompleter(m_BbCompleterModel);
    completer->setFilterMode(Qt::MatchContains);
    ui->comboBuildingBlocks->setModel(m_listBbModel);
    ui->comboBuildingBlocks->setCompleter(completer);
}

BuildingBlockMgrDlg::~BuildingBlockMgrDlg()
{
    delete ui;
}

void BuildingBlockMgrDlg::updateBuildingBlockChildren()
{
}
