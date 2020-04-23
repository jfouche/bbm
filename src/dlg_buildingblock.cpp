#include "dlg_buildingblock.h"
#include "ui_dlg_buildingblock.h"

BuildingBlockEditDlg::BuildingBlockEditDlg(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::AddBuildingBlockDlg)
{
    ui->setupUi(this);
}

BuildingBlockEditDlg::~BuildingBlockEditDlg()
{
    delete ui;
}

void BuildingBlockEditDlg::on_accepted()
{
    const QString name = ui->nameEdit->text();
    const QString ref = ui->refEdit->text();
    emit newBuildingBlock(name, ref);
}
