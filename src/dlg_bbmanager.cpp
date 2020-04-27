#include "dlg_bbmanager.h"
#include "ui_dlg_bbmanager.h"
#include "datamodel.h"
#include "model_bblist.h"


BuildingBlockMgrDlg::BuildingBlockMgrDlg(DataModel* model, QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::BuildingBlockMgrDlg)
    , m_model(model)
{
    ui->setupUi(this);

    m_listBbModel = new BuildingBlockListModel(m_model, this);
    ui->listBuildingBlocksChildren->setModel(m_listBbModel);
    ui->comboBuildingBlocks->setModel(m_listBbModel);
}

BuildingBlockMgrDlg::~BuildingBlockMgrDlg()
{
    delete ui;
}
