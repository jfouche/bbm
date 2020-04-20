#include "addprojectdlg.h"
#include "ui_addprojectdlg.h"

AddProjectDlg::AddProjectDlg(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::AddProjectDlg)
{
    ui->setupUi(this);
}

AddProjectDlg::~AddProjectDlg()
{
    delete ui;
}


void AddProjectDlg::on_buttonBox_accepted()
{
    const QString name = ui->editProjectName->text();
    emit newProjectName(name);
}
