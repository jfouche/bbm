#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "dlg_project.h"
#include "dlg_buildingblock.h"
#include "dlg_bbmanager.h"
#include <QDebug>
#include <QFileDialog>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ui->treeBuildingBlocks->setHeaderLabels({"Name", "Ref"});

    m_datamodel = new DataModel(this);

    // the project list model
    projectListModel = new ProjectListModel(this, m_datamodel);
    ui->listProjects->setModel(projectListModel);
    connect(m_datamodel, SIGNAL(dbChanged()), this, SLOT(updateBuildingBlockList()));
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
    auto root = ui->treeBuildingBlocks->invisibleRootItem();

    // clear the tree
    for (int i = 0; i < root->childCount(); ++i) {
        root->removeChild(root->child(i));
    }

    // add all building blocks
    for (int i = 0; i < m_datamodel->getBuildingBlockCount(); ++i) {
        BuildingBlock* bb = m_datamodel->getBuildingBlock(i);
        if (bb) {
            QStringList cols = { bb->name(), bb->ref() };
            auto item = new QTreeWidgetItem(ui->treeBuildingBlocks, cols);
            QVariant data(QVariant::fromValue(static_cast<void*>(bb)));
            item->setData(0, QTreeWidgetItem::UserType, data);
            root->addChild(item);
        }
    }
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
    BuildingBlockMgrDlg dlg(this);
    dlg.exec();
}

void MainWindow::editProject(const QModelIndex &index)
{
    Project* project = projectListModel->getProject(index);
    if (project) {
        ProjectEditDlg dlg(this);
        dlg.setProject(*project);
        if (dlg.exec() == QDialog::Accepted) {
            project->setName(dlg.getName());
        }
    }
}
