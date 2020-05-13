#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "datamodel.h"
#include "model_projectlist.h"
#include "model_bblist.h"
#include "model_bbtree.h"
#include "model_availablebbchildren.h"
#include "model_projectbblist.h"
#include <QFileDialog>
#include <QMessageBox>
#include <QStringListModel>

// ===========================================================================

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    m_datamodel = new DataModel(this);
    // TODO : remove me
    m_datamodel->load("test.json");

    /// listProjects
    projectListModel = new ProjectListModel(this, m_datamodel);
    filteredProjectListModel = new QSortFilterProxyModel(this);
    filteredProjectListModel->setSourceModel(projectListModel);
    ui->listProjects->setModel(filteredProjectListModel);

    /// listBuildingBlocks
    bbListModel = new BuildingBlockListModel(m_datamodel, this);
    filteredBbListModel = new QSortFilterProxyModel(this);
    filteredBbListModel->setSourceModel(bbListModel);
    ui->listBuildingBlocks->setModel(filteredBbListModel);

    /// treeBuildingBlocks
    bbTreeModel = new BuildingBlockTreeModel(m_datamodel, this);
    filteredBbTreeModel = new QSortFilterProxyModel(this);
    filteredBbTreeModel->setSourceModel(bbTreeModel);
    ui->treeBuildingBlocks->setModel(filteredBbTreeModel);

    /// listBbChildren
    availableBbChildrenModel = new AvailableBuildingBlockChildrenModel(bbListModel, this);
    ui->listBbChildren->setModel(availableBbChildrenModel);

    /// listBbInProject
    projectBbListModel = new ProjectBuildingBlockListModel(m_datamodel, this);
    ui->listBbInProject->setModel(projectBbListModel);

    /// Filter
    filteredProjectListModel->setFilterKeyColumn(-1);
    filteredBbListModel->setFilterKeyColumn(-1);
    filteredBbTreeModel->setFilterKeyColumn(-1);

    ///
    QStringListModel *model = new QStringListModel();
    QStringList list = {"A", "B", "C", "D", "E", "F"};
    model->setStringList(list);
    ui->comboBbMaturity->setModel(model);

    // SIGNALS
    connect(ui->btnLoad, &QPushButton::clicked, this, &MainWindow::load);
    connect(ui->btnSave, &QPushButton::clicked, this, &MainWindow::save);
    connect(ui->editFilter, &QLineEdit::textChanged, this, &MainWindow::filter);

    connect(ui->btnAddProject, &QPushButton::clicked, this, &MainWindow::addProject);
    connect(ui->btnEditProject, &QPushButton::clicked, this, &MainWindow::editCurrentProject);
    connect(ui->btnDelProject, &QPushButton::clicked, this, &MainWindow::delCurrentProject);

    connect(ui->btnAddBb, &QPushButton::clicked, this, &MainWindow::addBuildingBlock);
    connect(ui->btnEditBb, &QPushButton::clicked, this, &MainWindow::editCurrentBuildingBlock);
    connect(ui->btnDelBb, &QPushButton::clicked, this, &MainWindow::delCurrentBuildingBlock);

    connect(ui->editProjectName, &QLineEdit::editingFinished, this, &MainWindow::saveProject);
    connect(ui->btnBoxEditProject->button(QDialogButtonBox::Close), &QPushButton::clicked, this, &MainWindow::hideRightPanel);

    connect(ui->editBbName, &QLineEdit::editingFinished, this, &MainWindow::saveBuildingBlock);
    connect(ui->editBbRef, &QLineEdit::editingFinished, this, &MainWindow::saveBuildingBlock);
    connect(ui->editBbInfo, &QLineEdit::editingFinished, this, &MainWindow::saveBuildingBlock);
    connect(ui->btnBoxEditBb->button(QDialogButtonBox::Close), &QPushButton::clicked, this, &MainWindow::hideRightPanel);
    connect(ui->comboBbMaturity, qOverload<int>(&QComboBox::activated), [=](int){ saveBuildingBlock(); });

    auto onBbSelected = [this](const QItemSelection &selected, const QItemSelection &deselected) {
        Q_UNUSED(deselected)
        if (selected.indexes().empty() == false) {
            auto bbIndex = filteredBbListModel->mapToSource(selected.indexes().first());
            select(bbListModel->getBuildingBlock(bbIndex));
        }
    };
    connect(ui->listBuildingBlocks->selectionModel(), &QItemSelectionModel::selectionChanged, onBbSelected);

    auto onProjectSelected = [this](const QItemSelection &selected, const QItemSelection &deselected) {
        Q_UNUSED(deselected)
        if (selected.indexes().empty() == false) {
            auto bbIndex = filteredProjectListModel->mapToSource(selected.indexes().first());
            select(projectListModel->getProject(bbIndex));
        }
    };
    connect(ui->listProjects->selectionModel(), &QItemSelectionModel::selectionChanged, onProjectSelected);

    hideRightPanel();
    updateUI();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::hideRightPanel()
{
    ui->treeBuildingBlocks->setFocus();
    ui->rightWidget->hide();
    ui->editProjectName->setText(QString());
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

void MainWindow::updateUI()
{
    bool projectSelected = getSelectedProject();
    ui->btnEditProject->setEnabled(projectSelected);
    ui->btnDelProject->setEnabled(projectSelected);

    bool bbSelected = getSelectedBuildingBlock();
    ui->btnEditBb->setEnabled(bbSelected);
    ui->btnDelBb->setEnabled(bbSelected);
}

void MainWindow::filter(const QString& filter)
{
    QRegExp regex(filter, Qt::CaseInsensitive);
    if (ui->wdgEditBb->isVisible()) {
        availableBbChildrenModel->setFilterRegExp(regex);
    }
    else if (ui->wdgEditProject->isVisible()) {

    }
    else {
        filteredProjectListModel->setFilterRegExp(regex);
        filteredBbListModel->setFilterRegExp(regex);
        filteredBbTreeModel->setFilterRegExp(regex);
    }
}

void MainWindow::select(BuildingBlock* bb)
{
    ui->listProjects->selectionModel()->clear();
    updateUI();
    if (bb && ui->wdgEditBb->isVisible()) {
        editBuildingBlock(*bb);
    }
    else {
        hideRightPanel();
    }
}

void MainWindow::select(Project* project)
{
    ui->listBuildingBlocks->selectionModel()->clear();
    updateUI();
    if (project && ui->wdgEditProject->isVisible()) {
        editProject(*project);
    }
    else {
        hideRightPanel();
    }
}

void MainWindow::addProject()
{
    Project* project = m_datamodel->addProject();
    /// TODO : fixme
//    int size = m_datamodel->projects().size();
//    QModelIndex projIdx = projectListModel->index(size-1);
//    ui->listProjects->selectionModel()->select(projIdx, QItemSelectionModel::SelectCurrent);
    editProject(*project);
}

void MainWindow::editCurrentProject()
{
    Project* project = getSelectedProject();
    if (project) {
        editProject(*project);
    }
}

void MainWindow::editProject(Project& project)
{
    ui->rightWidget->show();
    ui->wdgEditBb->hide();
    ui->wdgEditProject->show();
    ui->editProjectName->setText(project.name());
    projectBbListModel->setProject(&project);
    ui->editProjectName->setFocus();
}

void MainWindow::delCurrentProject()
{
    Project* project = getSelectedProject();
    if (!project)
        return;

    m_datamodel->deleteProject(project);
    ui->listProjects->selectionModel()->clear();
}

void MainWindow::saveProject()
{
    Project* project = getSelectedProject();
    if (!project)
        return;
    project->setName(ui->editProjectName->text());
}

Project* MainWindow::getSelectedProject()
{
    auto sel = ui->listProjects->selectionModel()->currentIndex();
    if (sel.isValid() == false) {
        return nullptr;
    }
    auto idx = filteredProjectListModel->mapToSource(sel);
    return projectListModel->getProject(idx);
}

BuildingBlock* MainWindow::getSelectedBuildingBlock()
{
    auto sel = ui->listBuildingBlocks->selectionModel()->currentIndex();
    if (sel.isValid() == false) {
        return nullptr;
    }
    auto idx = filteredBbListModel->mapToSource(sel);
    return bbListModel->getBuildingBlock(idx);
}

void MainWindow::addBuildingBlock()
{
    BuildingBlock* bb = m_datamodel->addBuildingBlock();
    /// TODO : select it
    editBuildingBlock(*bb);
}

void MainWindow::editCurrentBuildingBlock()
{
    BuildingBlock* bb = getSelectedBuildingBlock();
    if (bb) {
        editBuildingBlock(*bb);
    }
}

void MainWindow::editBuildingBlock(BuildingBlock& bb)
{
    ui->rightWidget->show();
    ui->wdgEditBb->show();
    ui->wdgEditProject->hide();
    ui->editBbName->setText(bb.name());
    ui->editBbRef->setText(bb.ref());
    ui->comboBbMaturity->setCurrentIndex(bb.maturity());
    ui->editBbInfo->setText(bb.info());
    ui->editProjectName->setFocus();
    availableBbChildrenModel->setCurrentBuildingBlock(&bb);
}

void MainWindow::delCurrentBuildingBlock()
{
    BuildingBlock* bb = getSelectedBuildingBlock();
    if (!bb)
        return;

    if (m_datamodel->deleteBuildingBlock(bb) == false) {
        QMessageBox msgBox;
        msgBox.setIcon(QMessageBox::Warning);
        msgBox.setText("Can't delete building block because it is in use in project or building block.");
        msgBox.exec();
    }
}

void MainWindow::saveBuildingBlock()
{
    BuildingBlock* bb = getSelectedBuildingBlock();
    if (!bb)
        return;
    bb->setName(ui->editBbName->text());
    bb->setRef(ui->editBbRef->text());
    bb->setMaturity(static_cast<BuildingBlock::Maturity>(ui->comboBbMaturity->currentIndex()));
    bb->setInfo(ui->editBbInfo->text());
}
