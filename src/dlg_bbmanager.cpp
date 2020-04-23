#include "dlg_bbmanager.h"
#include "ui_dlg_bbmanager.h"

BuildingBlockMgrDlg::BuildingBlockMgrDlg(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::BuildingBlockMgrDlg)
{
    ui->setupUi(this);
}

BuildingBlockMgrDlg::~BuildingBlockMgrDlg()
{
    delete ui;
}
