#include "page_buildingblock.h"
#include "ui_page_buildingblock.h"
#include "model_bbtree.h"
#include "dlg_buildingblock.h"
#include "dlg_bbmanager.h"

FilteredBbTreeModel::FilteredBbTreeModel(BuildingBlockTreeModel* model, QObject* parent)
    : QSortFilterProxyModel(parent)
{
    setSourceModel(model);
}

BuildingBlockPage::BuildingBlockPage(DataModel* model, QWidget *parent) :
    QWidget(parent),
    ui(new Ui::BuildingBlockPage),
    m_model(model)
{
    ui->setupUi(this);

    m_bbTreeModel = new BuildingBlockTreeModel(m_model, this);
    m_filteredBbTreeModel = new FilteredBbTreeModel(m_bbTreeModel, this);
    m_filteredBbTreeModel->setFilterKeyColumn(-1);
    ui->treeBuildingBlocks->setModel(m_filteredBbTreeModel);

    // SIGNALS
    connect(ui->editFilter, SIGNAL(textChanged(QString)), m_filteredBbTreeModel, SLOT(setFilterRegularExpression(QString)));
    connect(ui->btnAddBuildingBlock, SIGNAL(clicked()), this, SLOT(addBuildingBlock()));
    connect(ui->btnEditBuildingBlock, SIGNAL(clicked()), this, SLOT(editCurrentBuildingBlock()));
    connect(ui->btnManageBuildingBlocks, SIGNAL(clicked()), this, SLOT(showBuildingBlockManager()));
    connect(ui->treeBuildingBlocks->selectionModel(), SIGNAL(selectionChanged(QItemSelection , QItemSelection )), this, SLOT(updateUI()));

    updateUI();
}

BuildingBlockPage::~BuildingBlockPage()
{
    delete ui;
}

void BuildingBlockPage::updateUI()
{
    auto curSel = ui->treeBuildingBlocks->selectionModel()->selection().indexes();
    bool sel = !curSel.empty();
    ui->btnEditBuildingBlock->setEnabled(sel);
    ui->btnDelBuildingBlock->setEnabled(sel);
}

void BuildingBlockPage::addBuildingBlock()
{
    BuildingBlockEditDlg dlg(this);
    if (dlg.exec() == QDialog::Accepted) {
        auto bb = m_model->addBuildingBlock();
        bb->setName(dlg.getName());
        bb->setRef(dlg.getRef());
    }
}

void BuildingBlockPage::editCurrentBuildingBlock()
{
    auto curSel = ui->treeBuildingBlocks->selectionModel()->selection().indexes();
    if (curSel.empty()) {
        return;
    }
    auto sel = ui->treeBuildingBlocks->selectionModel()->currentIndex();
    auto idx = m_filteredBbTreeModel->mapToSource(sel);
    auto bb = m_bbTreeModel->treeItem(idx)->data();
    if (bb) {
        BuildingBlockEditDlg dlg(this);
        dlg.set(*bb);
        if (dlg.exec() == QDialog::Accepted) {
            bb->setName(dlg.getName());
            bb->setRef(dlg.getRef());
            bb->setInfo(dlg.getInfo());
            bb->setMaturity(dlg.getMaturity());
        }
    }
}

void BuildingBlockPage::showBuildingBlockManager()
{
    BuildingBlockMgrDlg dlg(m_model, this);
    dlg.exec();
}
