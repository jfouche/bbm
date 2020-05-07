#include "dlg_projectbb.h"
#include "model_projectlist.h"
#include "ui_dlg_projectbb.h"
#include <QCompleter>

ProjectBuildingBlocksDlg::ProjectBuildingBlocksDlg(DataModel* model, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ProjectBuildingBlocksDlg),
    m_model(model)
{
    ui->setupUi(this);

    m_projectListModel = new ProjectListModel(this, m_model);
//    QCompleter* completer = new QCompleter(m_projectListModel);
//    completer->setFilterMode(Qt::MatchContains);
//    ui->comboProjects->setCompleter(completer);
    ui->comboProjects->setModel(m_projectListModel);
    ui->comboProjects->setCurrentIndex(0);
}

ProjectBuildingBlocksDlg::~ProjectBuildingBlocksDlg()
{
    delete ui;
}
