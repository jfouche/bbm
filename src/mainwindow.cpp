#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "dlg_project.h"
#include "dlg_buildingblock.h"
#include <QDebug>
#include <QFileDialog>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    m_datamodel = new DataModel(this);

    // the project list model
    projectListModel = new ProjectListModel(this, m_datamodel);
    ui->listProjects->setModel(projectListModel);
    connect(m_datamodel, SIGNAL(dbChanged()), this, SLOT(updateProjectList()));
}

MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::addProject()
{
    ProjectEditDlg dlg(this);
    if (dlg.exec() == QDialog::Accepted) {
        addProject(dlg.getName());
    }
}

void MainWindow::addBuildingBlock()
{
    BuildingBlockEditDlg dlg(this);
    if (dlg.exec() == QDialog::Accepted) {
        ;
    }
}

void MainWindow::addProject(QString name)
{
    m_datamodel->addProject(name);
}

void MainWindow::addBuildBlock(QString name, QString ref)
{
    m_datamodel->addBuildingBlock(name, ref);
}

void MainWindow::updateProjectList()
{

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

void MainWindow::editProject(const QModelIndex &index)
{
    ProjectEditDlg dlg(this);
    Project* project = projectListModel->getProject(index);
    if (project) {
        dlg.setProject(*project);
    }
    if (dlg.exec() == QDialog::Accepted) {
        project->setName(dlg.getName());
    }
}
