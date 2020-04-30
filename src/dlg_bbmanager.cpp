#include "dlg_bbmanager.h"
#include "ui_dlg_bbmanager.h"
#include "datamodel.h"
#include "model_bblist.h"
#include "model_bbtree.h"

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

AvailableBuildingBlockChildrenModel::AvailableBuildingBlockChildrenModel(BuildingBlockListModel* model, QObject* parent)
    : QSortFilterProxyModel(parent)
    , m_model(model)
    , m_parentBB(nullptr)
{
    setSourceModel(m_model);
}

AvailableBuildingBlockChildrenModel::AvailableBuildingBlockChildrenModel(DataModel* datamodel, QObject* parent)
    : AvailableBuildingBlockChildrenModel(new BuildingBlockListModel(datamodel, parent), parent)
{
}

void AvailableBuildingBlockChildrenModel::setParentBuildingBlock(BuildingBlock* parentBb)
{
    beginResetModel();
    m_parentBB = parentBb;
    endResetModel();
}

bool AvailableBuildingBlockChildrenModel::filterAcceptsRow(int sourceRow, const QModelIndex &sourceParent) const
{
    Q_UNUSED(sourceParent)
    if (m_model->getBuildingBlock(createIndex(sourceRow, 0)) == m_parentBB)
        return false;
    return true;
}

Qt::ItemFlags AvailableBuildingBlockChildrenModel::flags(const QModelIndex &index) const
{
    auto flags = QSortFilterProxyModel::flags(index);
    if (index.isValid()) {
        flags |= Qt::ItemIsUserCheckable;
    }
    return flags;
}

QVariant AvailableBuildingBlockChildrenModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid())
        return QVariant();

    auto srcIndex = mapToSource(index);
    auto childBb = m_model->getBuildingBlock(srcIndex);

    if(role == Qt::CheckStateRole) {
        qDebug() << "data - " << m_parentBB->name() << " > " << childBb->name() << " : " << m_parentBB->children().contains(childBb);
        if (m_parentBB->children().contains(childBb)) {
            return Qt::Checked;
        }
        else {
            return Qt::Unchecked;
        }
    }

    return QSortFilterProxyModel::data(index, role);
}

bool AvailableBuildingBlockChildrenModel::setData(const QModelIndex &index, const QVariant &value, int role)
{
    if(!index.isValid() || role != Qt::CheckStateRole)
            return false;

    auto srcIndex = mapToSource(index);
    auto childBb = m_model->getBuildingBlock(srcIndex);

    if(value == Qt::Checked) {
        qDebug() << m_parentBB->name() << " add " << childBb->name();
        m_parentBB->add(childBb);
    }
    else {
        qDebug() << m_parentBB->name() << " remove " << childBb->name();
        m_parentBB->remove(childBb);
    }
    emit dataChanged(index, index);
    return true;
}

// ===========================================================================

BuildingBlockMgrDlg::BuildingBlockMgrDlg(DataModel* model, QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::BuildingBlockMgrDlg)
    , m_model(model)
{
    ui->setupUi(this);

    // MODELS
    m_BbListModel = new BuildingBlockListModel(m_model, this);
    m_childrenBbModel = new AvailableBuildingBlockChildrenModel(m_BbListModel, this);
    m_BbCompleterModel = new BuildingBlocksCompleterModel(m_model, this);
    m_currentBbTreeModel = new BuildingBlockTreeModel(m_model, this);

    // WIDGETS config
    QCompleter* completer = new QCompleter(m_BbCompleterModel);
    completer->setFilterMode(Qt::MatchContains);
    ui->comboBuildingBlocks->setCompleter(completer);

    // UI models
    ui->listBuildingBlocksChildren->setModel(m_childrenBbModel);
    ui->treeCurrentBb->setModel(m_currentBbTreeModel);
    ui->comboBuildingBlocks->setModel(m_BbListModel);

    // SIGNALS
    connect(ui->comboBuildingBlocks, SIGNAL(activated(int)), this, SLOT(updateBuildingBlockChildren()));

    // INIT
    updateBuildingBlockChildren();
}

BuildingBlockMgrDlg::~BuildingBlockMgrDlg()
{
    delete ui;
}

void BuildingBlockMgrDlg::updateBuildingBlockChildren()
{

    int i = ui->comboBuildingBlocks->currentIndex();
    QModelIndex idx = m_BbListModel->index(i, 0);
    auto bb = m_BbListModel->getBuildingBlock(idx);
    m_childrenBbModel->setParentBuildingBlock(bb);
}
