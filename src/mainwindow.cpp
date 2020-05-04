#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "dlg_project.h"
#include "dlg_buildingblock.h"
#include "dlg_bbmanager.h"
#include "datamodel.h"
#include "model_projectlist.h"
#include "model_bbtree.h"
#include <QDebug>
#include <QFileDialog>

// ===========================================================================

FilteredBbTreeModel::FilteredBbTreeModel(BuildingBlockTreeModel* model, QObject* parent)
    : QSortFilterProxyModel(parent)
{
    setSourceModel(model);
}

// ===========================================================================

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    m_datamodel = new DataModel(this);
    m_projectListModel = new ProjectListModel(this, m_datamodel);
    m_bbTreeModel = new BuildingBlockTreeModel(m_datamodel, this);

    // TODO : remove me
    m_datamodel->load("test.json");

    // the project list model
    ui->listProjects->setModel(m_projectListModel);

    // the BB tree model
    m_filteredBbTreeModel = new FilteredBbTreeModel(m_bbTreeModel, this);
    m_filteredBbTreeModel->setFilterKeyColumn(-1);
    ui->treeBuildingBlocks->setModel(m_filteredBbTreeModel);

    // SIGNALS
    connect(ui->editFilter, SIGNAL(textChanged(QString)), m_filteredBbTreeModel, SLOT(setFilterRegularExpression(QString)));
    connect(ui->btnEditBuildingBlock, SIGNAL(clicked()), this, SLOT(editCurrentBuildingBlock()));
    connect(ui->treeBuildingBlocks->selectionModel(), SIGNAL(selectionChanged(QItemSelection , QItemSelection )), this, SLOT(updateUI()));

    updateUI();
}

MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::addProject()
{
    ProjectEditDlg dlg(this);
    if (dlg.exec() == QDialog::Accepted) {
        auto project = m_datamodel->addProject();
        project->setName(dlg.getName());
    }
}

void MainWindow::addBuildingBlock()
{
    BuildingBlockEditDlg dlg(this);
    if (dlg.exec() == QDialog::Accepted) {
        auto bb = m_datamodel->addBuildingBlock();
        bb->setName(dlg.getName());
        bb->setRef(dlg.getRef());
    }
}

void MainWindow::updateProjectList()
{
}

void MainWindow::updateBuildingBlockList()
{
}

void MainWindow::updateUI()
{
    auto curSel = ui->treeBuildingBlocks->selectionModel()->selection().indexes();
    bool sel = !curSel.empty();
    ui->btnEditBuildingBlock->setEnabled(sel);
    ui->btnDelBuildingBlock->setEnabled(sel);
}

void MainWindow::load()
{
    QString filename = QFileDialog::getOpenFileName(this, tr("Open file"), "", tr("BB Files (*.json)"));
    if (!filename.isNull()) {
        m_datamodel->load(filename);
    }
}

void MainWindow::save()
{
    QString filename = QFileDialog::getSaveFileName(this, tr("Open file"), "", tr("BB Files (*.json)"));
    if (!filename.isNull()) {
        m_datamodel->save(filename);
    }
}

void MainWindow::showProjectPage()
{
    ui->stackedWidget->setCurrentIndex(0);
}

void MainWindow::showBuildingBlockPage()
{
    ui->stackedWidget->setCurrentIndex(1);
}

void MainWindow::showBuildingBlockManager()
{
    BuildingBlockMgrDlg dlg(m_datamodel, this);
    dlg.exec();
}

void MainWindow::editProject(const QModelIndex &index)
{
    Project* project = m_projectListModel->getProject(index);
    if (project) {
        ProjectEditDlg dlg(this);
        dlg.setProject(*project);
        if (dlg.exec() == QDialog::Accepted) {
            project->setName(dlg.getName());
        }
    }
}

void MainWindow::editCurrentBuildingBlock()
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
