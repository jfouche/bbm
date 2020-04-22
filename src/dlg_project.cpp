#include "dlg_project.h"
#include "ui_addprojectdlg.h"

ProjectEditDlg::ProjectEditDlg(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::ProjectDlg)
{
    ui->setupUi(this);
}

ProjectEditDlg::~ProjectEditDlg()
{
    delete ui;
}

void ProjectEditDlg::setProject(const Project &project)
{
    ui->editProjectName->setText(project.name());
}

QString ProjectEditDlg::getName() const
{
    return ui->editProjectName->text();
}

