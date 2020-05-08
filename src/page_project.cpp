#include "datamodel.h"
#include "ui_page_project.h"
#include "model_projectlist.h"
#include "page_project.h"
#include "dlg_project.h"

ProjectPage::ProjectPage(DataModel* model, QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ProjectPage),
    m_model(model)
{
    ui->setupUi(this);

    m_projectListModel = new ProjectListModel(this, m_model);
    ui->listProjects->setModel(m_projectListModel);

    connect(ui->btnAddProject, &QPushButton::clicked, this, &ProjectPage::addProject);
}

ProjectPage::~ProjectPage()
{
    delete ui;
}

void ProjectPage::editCurrentProject()
{
//    Project* project = m_projectListModel->getProject(index);
//    if (project) {
//        ProjectEditDlg dlg(this);
//        dlg.setProject(*project);
//        if (dlg.exec() == QDialog::Accepted) {
//            project->setName(dlg.getName());
//        }
//    }
}

void ProjectPage::addProject()
{
    ProjectEditDlg dlg(this);
    if (dlg.exec() == QDialog::Accepted) {
        auto project = m_model->addProject();
        project->setName(dlg.getName());
    }
}

