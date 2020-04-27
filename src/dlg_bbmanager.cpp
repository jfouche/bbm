#include "dlg_bbmanager.h"
#include "ui_dlg_bbmanager.h"
#include "datamodel.h"
#include "model_bblist.h"


BuildingBlockMgrDlg::BuildingBlockMgrDlg(DataModel* datamodel, QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::BuildingBlockMgrDlg)
    , m_datamodel(datamodel)
{
    ui->setupUi(this);

    m_listBbModel = new BuildingBlockListModel(m_datamodel, this);
    ui->listAllBuildingBlocks->setModel(m_listBbModel);
    //ui->comboBox->setModel(m_listBbModel);
}

BuildingBlockMgrDlg::~BuildingBlockMgrDlg()
{
    delete ui;
}
