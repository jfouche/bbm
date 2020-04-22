#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "addprojectdlg.h"
#include "addbuildingblockdlg.h"
#include "projectlistmodel.h"
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
    ui->listProjects->setModel(new ProjectListModel(this, m_datamodel));

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
    AddProjectDlg* dlg = new AddProjectDlg(this);
    connect(dlg, SIGNAL(newProjectName(QString)), this, SLOT(addProject(QString)));
    dlg->exec();
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
