#include "page_buildingblock.h"
#include "ui_page_buildingblock.h"
#include "model_bbtree.h"
#include "dlg_buildingblock.h"
#include "dlg_bbmanager.h"

#include <QMessageBox>

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
    connect(ui->editFilter, &QLineEdit::textChanged, this, &BuildingBlockPage::filter);
    connect(ui->btnAddBuildingBlock, &QPushButton::clicked, this, &BuildingBlockPage::addBuildingBlock);
    connect(ui->btnEditBuildingBlock, &QPushButton::clicked, this, &BuildingBlockPage::editCurrentBuildingBlock);
    connect(ui->btnDelBuildingBlock, &QPushButton::clicked, this, &BuildingBlockPage::removeCurrentBuildingBlock);
    connect(ui->btnManageBuildingBlocks, &QPushButton::clicked, this, &BuildingBlockPage::showBuildingBlockManager);
    connect(ui->treeBuildingBlocks->selectionModel(), &QItemSelectionModel::selectionChanged, this, &BuildingBlockPage::updateUI);

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

void BuildingBlockPage::filter(const QString& filter)
{
    m_filteredBbTreeModel->setFilterRegularExpression(filter);
}

BuildingBlock* BuildingBlockPage::getSelection()
{
    auto curSel = ui->treeBuildingBlocks->selectionModel()->selection().indexes();
    if (curSel.empty()) {
        return nullptr;
    }
    auto sel = ui->treeBuildingBlocks->selectionModel()->currentIndex();
    auto idx = m_filteredBbTreeModel->mapToSource(sel);
    return m_bbTreeModel->treeItem(idx)->data();
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
    auto bb = getSelection();
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

void BuildingBlockPage::removeCurrentBuildingBlock()
{
    auto bb = getSelection();
    if (bb) {
        if (m_model->deleteBuildingBlock(bb) == false) {
            QMessageBox msgBox;
            msgBox.setIcon(QMessageBox::Warning);
            msgBox.setText("Can't delete building block because it is in use in project or building block.");
            msgBox.exec();
        }
    }
}

void BuildingBlockPage::showBuildingBlockManager()
{
    BuildingBlockMgrDlg dlg(m_model, this);
    dlg.exec();
}

