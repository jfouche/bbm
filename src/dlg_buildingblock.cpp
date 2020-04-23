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

void BuildingBlockEditDlg::set(const BuildingBlock &bb)
{
    ui->nameEdit->setText(bb.name());
    ui->refEdit->setText(bb.ref());
}

QString BuildingBlockEditDlg::getName() const
{
    return ui->nameEdit->text();
}

QString BuildingBlockEditDlg::getRef() const
{
    return ui->refEdit->text();
}

