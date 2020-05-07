#include "datamodel.h"
#include "ui_page_project.h"
#include "model_projectlist.h"
#include "page_project.h"
#include "dlg_project.h"
#include "dlg_projectbb.h"

ProjectPage::ProjectPage(DataModel* model, QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ProjectPage),
    m_model(model)
{
    ui->setupUi(this);

    m_projectListModel = new ProjectListModel(this, m_model);
    ui->listProjects->setModel(m_projectListModel);

    connect(ui->btnAddProject, SIGNAL(clicked()), this, SLOT(addProject()));
    connect(ui->btnDelete, SIGNAL(clicked()), this, SLOT(deleteCurrentProject()));
    connect(ui->btnEdit, SIGNAL(clicked()), this, SLOT(editCurrentProject()));
    connect(ui->btnAddBb, SIGNAL(clicked()), this, SLOT(addBuildingBlockToProject()));
}

ProjectPage::~ProjectPage()
{
    delete ui;
}

Project* ProjectPage::getSelection()
{
    auto curSel = ui->listProjects->selectionModel()->selection().indexes();
    if (curSel.empty()) {
        return nullptr;
    }
    auto sel = ui->listProjects->selectionModel()->currentIndex();
    //auto idx = m_projectListModel->mapToSource(sel);
    return m_projectListModel->getProject(sel);
}

void ProjectPage::addProject()
{
    ProjectEditDlg dlg(this);
    if (dlg.exec() == QDialog::Accepted) {
        auto project = m_model->addProject();
        project->setName(dlg.getName());
    }
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

void ProjectPage::deleteCurrentProject()
{
    auto project = getSelection();
    if (!project) {
        return;
    }
    m_model->deleteProject(project);
}

void ProjectPage::addBuildingBlockToProject()
{
    ProjectBuildingBlocksDlg dlg(m_model, this);
    dlg.exec();
}

