#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "datamodel.h"
#include "page_project.h"
#include "page_buildingblock.h"
#include "model_projectlist.h"
#include "model_bblist.h"
#include "model_bbtree.h"
#include "model_availablebbchildren.h"
#include <QFileDialog>

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

    //    m_pageProject = new ProjectPage(m_datamodel, this);
    //    m_pageBb = new BuildingBlockPage(m_datamodel, this);

    connect(ui->btnLoad, &QPushButton::clicked, this, &MainWindow::load);
    connect(ui->btnSave, &QPushButton::clicked, this, &MainWindow::save);
    connect(ui->editFilter, &QLineEdit::textChanged, this, &MainWindow::filter);
    connect(ui->btnAddProject, &QPushButton::clicked, this, &MainWindow::addProject);
    connect(ui->btnEditProject, &QPushButton::clicked, this, &MainWindow::editCurrentProject);
    connect(ui->btnDelProject, &QPushButton::clicked, this, &MainWindow::delCurrentProject);
    connect(ui->btnBoxEditProject->button(QDialogButtonBox::Save), &QPushButton::clicked, this, &MainWindow::saveProject);
    connect(ui->btnBoxEditProject->button(QDialogButtonBox::Cancel), &QPushButton::clicked, this, &MainWindow::hideRightPanel);

    auto onBbSelected = [this](const QItemSelection &selected, const QItemSelection &deselected) {
        Q_UNUSED(deselected)
        if (selected.indexes().empty() == false) {
            auto bbIndex = filteredBbListModel->mapToSource(selected.indexes().first());
            auto bb = bbListModel->getBuildingBlock(bbIndex);
            select(bb);
        }
    };
    connect(ui->listBuildingBlocks->selectionModel(), &QItemSelectionModel::selectionChanged, onBbSelected);

    auto onProjectSelected = [this](const QItemSelection &selected, const QItemSelection &deselected) {
        Q_UNUSED(deselected)
        if (selected.indexes().empty() == false) {
            auto bbIndex = filteredProjectListModel->mapToSource(selected.indexes().first());
            auto project = projectListModel->getProject(bbIndex);
            select(project);
        }
    };
    connect(ui->listProjects->selectionModel(), &QItemSelectionModel::selectionChanged, onProjectSelected);

    hideRightPanel();
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
    auto bbSelIdx = ui->listBuildingBlocks->selectionModel()->currentIndex();
    if (bbSelIdx.isValid()) {

    }
}

void MainWindow::filter(const QString& filter)
{
    QRegExp regex(filter, Qt::CaseInsensitive);
    filteredProjectListModel->setFilterRegExp(regex);
    filteredBbListModel->setFilterRegExp(regex);
    filteredBbTreeModel->setFilterRegExp(regex);
}

void MainWindow::select(BuildingBlock* bb)
{
    availableBbChildrenModel->setParentBuildingBlock(bb);
    ui->wdgEditBb->show();
    ui->wdgEditProject->hide();
}

void MainWindow::select(Project* project)
{
    //    availableBbChildrenModel->setParentBuildingBlock(bb);
    ui->wdgEditBb->hide();
    ui->wdgEditProject->show();
}

void MainWindow::addProject()
{
    Project* project = m_datamodel->addProject();
    select(project);
    ui->editProjectName->setFocus();
}

void MainWindow::editCurrentProject()
{
    Project* project = getSelectedProject();
    if (!project)
        return;

    ui->rightWidget->show();
    ui->wdgEditBb->hide();
    ui->wdgEditProject->show();
    ui->editProjectName->setText(project->name());
    ui->editProjectName->setFocus();
}

void MainWindow::delCurrentProject()
{
    Project* project = getSelectedProject();
    if (!project)
        return;

    m_datamodel->deleteProject(project);
}

void MainWindow::saveProject()
{
    Project* project = getSelectedProject();
    if (!project)
        return;
    project->setName(ui->editProjectName->text());
    ui->rightWidget->hide();
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
