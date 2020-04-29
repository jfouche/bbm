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

EditableBuildingBlocksModel::EditableBuildingBlocksModel(DataModel* datamodel, QObject* parent)
    : QAbstractListModel(parent)
    , m_model(datamodel)
{
}

bool EditableBuildingBlocksModel::setData(const QModelIndex &index, const QVariant &value, int role)
{
    qDebug() << "EditableBuildingBlocksModel::setData(idx, " << value << ")";
    bool ret = QAbstractListModel::setData(index, value, role);
    return ret;
}

bool EditableBuildingBlocksModel::setItemData(const QModelIndex &index, const QMap<int, QVariant> &roles)
{
    qDebug() << "EditableBuildingBlocksModel::setItemData(idx, ...)";
    bool ret = QAbstractListModel::setItemData(index, roles);
    return ret;
}

void EditableBuildingBlocksModel::filter(const QString& filter)
{
    qDebug() << "EditableBuildingBlocksModel::filter(" << filter << ")";
    beginResetModel();
    m_filter = filter;
    endResetModel();
}

const QString& EditableBuildingBlocksModel::filter() const
{
    return m_filter;
}

int EditableBuildingBlocksModel::rowCount(const QModelIndex &parent) const
{
    Q_ASSERT(parent.parent().isValid() == false);
    return m_model->buildingBlocks().size() + 1;
}

int EditableBuildingBlocksModel::columnCount(const QModelIndex& parent) const
{
    Q_UNUSED(parent)
    return 1;
}

QVariant EditableBuildingBlocksModel::data(const QModelIndex &index, int role) const
{
    if (index.isValid() && (role == Qt::DisplayRole || role == Qt::EditRole)) {
        if (index.row() == 0) {
            return m_filter;
        }
        else {
            auto bb = m_model->buildingBlocks().at(index.row() - 1);
            return QString("%1 (%2)").arg(bb->name(), bb->ref());
        }
    }
    return QVariant();
}

Qt::ItemFlags EditableBuildingBlocksModel::flags(const QModelIndex &index) const
{
    if (!index.isValid()) {
        return Qt::NoItemFlags;
    }
    auto flags = QAbstractListModel::flags(index);
    if (index.row() == 0){
        flags |= Qt::ItemIsEditable;
    }
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

//    connect(ui->comboBuildingBlocks, SIGNAL(editTextChanged(QString)), this, SLOT(updateFilteredBuildingBlocks(QString)));
}

BuildingBlockMgrDlg::~BuildingBlockMgrDlg()
{
    delete ui;
}

void BuildingBlockMgrDlg::updateFilteredBuildingBlocks(const QString &filter)
{
//    m_proxyFilteredListBbModel->filter(filter);
//    m_listBbModel->filter(filter);
    m_BbCompleterModel->filter(filter);
}
