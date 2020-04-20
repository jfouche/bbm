#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "addprojectdlg.h"
#include "projectlistmodel.h"
#include <QDebug>

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

void MainWindow::addProject(QString name)
{
    m_datamodel->addProject(name);
}

void MainWindow::updateList()
{

}
