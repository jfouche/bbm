#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "datamodel.h"
#include "model_projectlist.h"
#include "model_bblist.h"
#include "model_detail_uses.h"
#include "model_detail_usedby.h"
#include "model_availablebbchildren.h"
#include "model_projectbblist.h"
#include <QFileDialog>
#include <QMessageBox>
#include <QStringListModel>

static const QString TITLE = "Building Block Manager";

static void decorateSplitter(QSplitter* splitter, int index)
{
    Q_ASSERT(splitter != NULL);

    const int gripLength = 25; 
    const int gripWidth = 1;
    const int grips = 3;

    splitter->setOpaqueResize(false);
    splitter->setChildrenCollapsible(false);

    splitter->setHandleWidth(7);
    QSplitterHandle* handle = splitter->handle(index);
    Qt::Orientation orientation = splitter->orientation();
    QHBoxLayout* layout = new QHBoxLayout(handle);
    layout->setSpacing(0);
    layout->setMargin(0);

    if (orientation == Qt::Horizontal)
    {
        for (int i=0;i<grips;++i)
        {
            QFrame* line = new QFrame(handle);
            line->setMinimumSize(gripWidth, gripLength);
            line->setMaximumSize(gripWidth, gripLength);
            line->setFrameShape(QFrame::StyledPanel);
            layout->addWidget(line);
        }
    }
    else
    {
        //this will center the vertical grip
        //add a horizontal spacer
        layout->addStretch();
        //create the vertical grip
        QVBoxLayout* vbox = new QVBoxLayout;
        for (int i=0;i<grips;++i)
        {
            QFrame* line = new QFrame(handle);
            line->setMinimumSize(gripLength, gripWidth);
            line->setMaximumSize(gripLength, gripWidth);
            line->setFrameShape(QFrame::StyledPanel);
            vbox->addWidget(line);
        }
        layout->addLayout(vbox);
        //add another horizontal spacer
        layout->addStretch();
    }
}

// ===========================================================================

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    m_datamodel = new DataModel(this);

    ui->setupUi(this);
    updateTitle();

    /// Splitter handle
    decorateSplitter(ui->splitter, 1);
 
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
    usesTreeModel = new UsesTreeModel(m_datamodel, this);
    usedByTreeModel = new UsedByTreeModel(m_datamodel, this);
    filteredDetailTreeModel = new QSortFilterProxyModel(this);
    filteredDetailTreeModel->setSourceModel(usesTreeModel);
    ui->treeDetail->setModel(filteredDetailTreeModel);

    /// listBbChildren
    availableBbChildrenModel = new AvailableBuildingBlockChildrenModel(bbListModel, this);
    ui->listBbChildren->setModel(availableBbChildrenModel);

    /// listBbInProject
    projectBbListModel = new ProjectBuildingBlockListModel(m_datamodel, this);
    ui->listBbInProject->setModel(projectBbListModel);

    /// Filter
    filteredProjectListModel->setFilterKeyColumn(-1);
    filteredBbListModel->setFilterKeyColumn(-1);
    filteredDetailTreeModel->setFilterKeyColumn(-1);

    ///
    QStringListModel *model = new QStringListModel();
    QStringList list = {"A", "B", "C", "D", "E", "F"};
    model->setStringList(list);
    ui->comboBbMaturity->setModel(model);

    /// ACTIONS
    const QStyle* s = style();
    //ui->action_New->setIcon(s->standardIcon(QStyle::SP_new))
    ui->action_Open->setIcon(s->standardIcon(QStyle::SP_DialogOpenButton));

    // SIGNALS
    connect(ui->action_New, &QAction::triggered, this, &MainWindow::clear);
    connect(ui->action_Open, &QAction::triggered, this, &MainWindow::load);
    connect(ui->action_Save, &QAction::triggered, this, &MainWindow::save);
    connect(ui->actionSave_as, &QAction::triggered, this, &MainWindow::saveAs);

    connect(ui->editFilter, &QLineEdit::textChanged, this, &MainWindow::filter);

    connect(ui->btnUses, &QRadioButton::clicked, this, &MainWindow::updateDetailModel);
    connect(ui->btnUsedBy, &QRadioButton::clicked, this, &MainWindow::updateDetailModel);

    connect(ui->btnAddProject, &QPushButton::clicked, this, &MainWindow::addProject);
    connect(ui->btnEditProject, &QPushButton::clicked, this, &MainWindow::editCurrentProject);
    connect(ui->btnDelProject, &QPushButton::clicked, this, &MainWindow::delCurrentProject);

    connect(ui->btnAddBb, &QPushButton::clicked, this, &MainWindow::addBuildingBlock);
    connect(ui->btnEditBb, &QPushButton::clicked, this, &MainWindow::editCurrentBuildingBlock);
    connect(ui->btnDelBb, &QPushButton::clicked, this, &MainWindow::delCurrentBuildingBlock);

    connect(ui->editProjectName, &QLineEdit::editingFinished, this, &MainWindow::saveProject);

    connect(ui->editBbName, &QLineEdit::editingFinished, this, &MainWindow::saveBuildingBlock);
    connect(ui->editBbRef, &QLineEdit::editingFinished, this, &MainWindow::saveBuildingBlock);
    connect(ui->editBbInfo, &QLineEdit::editingFinished, this, &MainWindow::saveBuildingBlock);
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

    ui->btnUses->setChecked(true);
    updateUI();
    updateDetailModel();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::clear() 
{
    m_filename.clear();
    m_datamodel->clear();
    updateTitle();

    ui->editFilter->clear();
    
    ui->editBbName->clear();
    ui->editBbRef->clear();
    ui->editBbInfo->clear();
    ui->comboBbMaturity->setCurrentIndex(0);

    ui->editProjectName->clear();
}

void MainWindow::load()
{
    clear();
    QString filename = QFileDialog::getOpenFileName(this, tr("Open file"), "", tr("BB Files (*.json)"));
    if (!filename.isNull()) {
        m_datamodel->load(filename);
        m_filename = filename;
        updateTitle();
    }
}

void MainWindow::save()
{
    if (m_filename.isEmpty()) {
        saveAs();
    }
    else
    {
        m_datamodel->save(m_filename);
        updateTitle();
    }
}

void MainWindow::saveAs()
{
    QString filename = QFileDialog::getSaveFileName(this, tr("Open file"), "", tr("BB Files (*.json)"));
    if (!filename.isNull()) {
        m_filename = filename;
        save();
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

void MainWindow::updateDetailModel()
{
    if (ui->btnUses->isChecked()) {
        filteredDetailTreeModel->setSourceModel(usesTreeModel);
    }
    else {
        filteredDetailTreeModel->setSourceModel(usedByTreeModel);
    }
}

void MainWindow::filter(const QString& filter)
{
    QRegExp regex(filter, Qt::CaseInsensitive);
    availableBbChildrenModel->setFilterRegExp(regex);
    filteredProjectListModel->setFilterRegExp(regex);
    filteredBbListModel->setFilterRegExp(regex);
    filteredDetailTreeModel->setFilterRegExp(regex);
}

void MainWindow::select(BuildingBlock* bb)
{
    ui->listProjects->selectionModel()->clear();
    usesTreeModel->set(bb);
    usedByTreeModel->set(bb);
    ui->treeDetail->expandAll();
    updateUI();
    if (bb && ui->dockEdit->isVisible()) {
        editBuildingBlock(bb);
    }
}

void MainWindow::select(Project* project)
{
    ui->listBuildingBlocks->selectionModel()->clear();
    usesTreeModel->set(project);
    usedByTreeModel->set(project);
    ui->treeDetail->expandAll();
    updateUI();
    if (project && ui->dockEdit->isVisible()) {
        editProject(project);
    }
}

void MainWindow::addProject()
{
    Project* project = m_datamodel->addProject();
    editProject(project);
    int pos = m_datamodel->projects().indexOf(project);
    QModelIndex projIdx = filteredProjectListModel->index(pos, 0);
    ui->listProjects->selectionModel()->select(projIdx, QItemSelectionModel::ClearAndSelect);
}

void MainWindow::editCurrentProject()
{
    Project* project = getSelectedProject();
    if (project) {
        editProject(project);
    }
}

void MainWindow::editProject(Project* project)
{
    ui->dockEdit->show();
    ui->groupProject->show();
    ui->groupBuildingBlock->hide();
    ui->editProjectName->setText(project->name());
    projectBbListModel->setProject(project);
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
    auto sel = ui->listProjects->selectionModel()->selectedIndexes();
    if (sel.empty()) {
        return nullptr;
    }
    auto idx = filteredProjectListModel->mapToSource(sel.at(0));
    return projectListModel->getProject(idx);
}

BuildingBlock* MainWindow::getSelectedBuildingBlock()
{
    auto sel = ui->listBuildingBlocks->selectionModel()->selectedIndexes();
    if (sel.empty()) {
        return nullptr;
    }
    auto idx = filteredBbListModel->mapToSource(sel.at(0));
    return bbListModel->getBuildingBlock(idx);
}

void MainWindow::addBuildingBlock()
{
    BuildingBlock* bb = m_datamodel->addBuildingBlock();
    editBuildingBlock(bb);
    int pos = m_datamodel->buildingBlocks().indexOf(bb);
    QModelIndex bbIdx = filteredBbListModel->index(pos, 0);
    ui->listBuildingBlocks->selectionModel()->select(bbIdx, QItemSelectionModel::ClearAndSelect);
}

void MainWindow::editCurrentBuildingBlock()
{
    BuildingBlock* bb = getSelectedBuildingBlock();
    if (bb) {
        editBuildingBlock(bb);
    }
}

void MainWindow::editBuildingBlock(BuildingBlock* bb)
{
    ui->dockEdit->show();
    ui->groupBuildingBlock->show();
    ui->groupProject->hide();
    ui->editBbName->setText(bb->name());
    ui->editBbRef->setText(bb->ref());
    ui->comboBbMaturity->setCurrentIndex(bb->maturity());
    ui->editBbInfo->setText(bb->info());
    ui->editBbName->setFocus();
    availableBbChildrenModel->setCurrentBuildingBlock(bb);
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

void MainWindow::updateTitle(bool modified)
{
    QString title(TITLE);
    if (!m_filename.isEmpty()) {
        title.append(" - ").append(m_filename);
    }
    if (modified) {
        title.append(" *");
    }
    setWindowTitle(title);
}