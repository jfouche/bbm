#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "dlg_project.h"
#include "addbuildingblockdlg.h"
#include <QDebug>
#include <QFileDialog>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    /*
    QStringList List;
    List << "Clair de Lune" << "Reverie" << "Prelude";
    model = new QStringListModel(this);
    model->setStringList(List);
    ui->listProjects->setModel(model);
*/

    m_datamodel = new DataModel(this);
    projectListModel = new ProjectListModel(this, m_datamodel);
    ui->listProjects->setModel(projectListModel);

    connect(m_datamodel, SIGNAL(dbChanged()), this, SLOT(updateList()));

    connect(ui->btnLoad, SIGNAL(clicked()), this, SLOT(load()));
    connect(ui->btnSave, SIGNAL(clicked()), this, SLOT(save()));
}

MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::on_btnAddProject_clicked()
{
    ProjectEditDlg dlg(this);
    if (dlg.exec() == QDialog::Accepted) {
        addProject(dlg.getName());
    }
}

void MainWindow::on_btnAddBuildinBlock_clicked()
{
    AddBuildingBlockDlg* dlg = new AddBuildingBlockDlg(this);
    connect(dlg, SIGNAL(newBuildingBlock(QString, Qstring)), this, SLOT(addBuildingBlock(QString, QString)));
    dlg->exec();
}

void MainWindow::addProject(QString name)
{
    m_datamodel->addProject(name);
}

void MainWindow::addBuildBlock(QString name, QString ref)
{
    m_datamodel->addBuildingBlock(name, ref);
}

void MainWindow::updateList()
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

void MainWindow::on_listProjects_doubleClicked(const QModelIndex &index)
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
