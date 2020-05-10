#include "dlg_bbmanager.h"
#include "ui_dlg_bbmanager.h"
#include "datamodel.h"
#include "model_bblist.h"
#include "model_bbtree.h"
#include "model_availablebbchildren.h"
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

CurrentBbTreeModel::CurrentBbTreeModel(BuildingBlockTreeModel* model, QObject* parent)
    : QSortFilterProxyModel(parent)
    , m_model(model)
    , m_parentBB(nullptr)
{
    setSourceModel(m_model);
}

CurrentBbTreeModel::CurrentBbTreeModel(DataModel* datamodel, QObject* parent)
    : CurrentBbTreeModel(new BuildingBlockTreeModel(datamodel, parent), parent)
{
}

void CurrentBbTreeModel::setParentBuildingBlock(BuildingBlock* parentBb)
{
    beginResetModel();
    m_parentBB = parentBb;
    endResetModel();
}

bool CurrentBbTreeModel::filterAcceptsRow(int sourceRow, const QModelIndex &sourceParent) const
{
    qDebug() << "CurrentBbTreeModel::filterAcceptsRow(" << sourceRow << ", " << sourceParent.row() << ") - selection = " << (m_parentBB ? m_parentBB->name() : "null");
    TreeItem* item = m_model->treeItem(sourceRow, sourceParent);
    if (item->data() == nullptr) {
        return true;
    }
    do {
        qDebug() << "  - item is : " << item->data()->name();
        if (item->data() == m_parentBB) {
            return true;
        }
        item = item->parentItem();
    }
    while (item && item->data());
    return false;
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
    m_childrenBbModel = nullptr; //new AvailableBuildingBlockChildrenModel(m_model, this);
    m_BbCompleterModel = new BuildingBlocksCompleterModel(m_model, this);
    m_currentBbTreeModel = new CurrentBbTreeModel(m_model, this);

    // WIDGETS config
    QCompleter* completer = new QCompleter(m_BbCompleterModel);
    completer->setFilterMode(Qt::MatchContains);
    ui->comboBuildingBlocks->setCompleter(completer);

    // UI models
    ui->listBuildingBlocksChildren->setModel(m_childrenBbModel);
    ui->treeCurrentBb->setModel(m_currentBbTreeModel);
    ui->comboBuildingBlocks->setModel(m_BbListModel);

    // SIGNALS
    connect(ui->comboBuildingBlocks, qOverload<int>(&QComboBox::activated), this, &BuildingBlockMgrDlg::setCurrentBuildingBlock);

    // INIT
    setCurrentBuildingBlock(0);
}

BuildingBlockMgrDlg::~BuildingBlockMgrDlg()
{
    delete ui;
}

void BuildingBlockMgrDlg::setCurrentBuildingBlock(int index)
{
    QModelIndex idx = m_BbListModel->index(index, 0);
    auto bb = m_BbListModel->getBuildingBlock(idx);

    m_childrenBbModel->setParentBuildingBlock(bb);
    m_currentBbTreeModel->setParentBuildingBlock(bb);

    ui->treeCurrentBb->expandAll();
}
