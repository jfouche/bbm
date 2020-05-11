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

    connect(ui->listBuildingBlocks->selectionModel(), &QItemSelectionModel::selectionChanged, [=](const QItemSelection &selected, const QItemSelection &deselected) {
        if (selected.indexes().empty() == false) {
            auto bbIndex = filteredBbListModel->mapToSource(selected.indexes().first());
            auto bb = bbListModel->getBuildingBlock(bbIndex);
            select(bb);
        }
    });

    connect(ui->listProjects->selectionModel(), &QItemSelectionModel::selectionChanged, [=](const QItemSelection &selected, const QItemSelection &deselected) {
        if (selected.indexes().empty() == false) {
            auto bbIndex = filteredProjectListModel->mapToSource(selected.indexes().first());
            auto project = projectListModel->getProject(bbIndex);
            select(project);
        }
    });

//    ui->rightWidget->hide();
}

MainWindow::~MainWindow()
{
    delete ui;
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
