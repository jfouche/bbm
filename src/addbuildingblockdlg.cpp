#include "addbuildingblockdlg.h"
#include "ui_addbuildingblockdlg.h"

AddBuildingBlockDlg::AddBuildingBlockDlg(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::AddBuildingBlockDlg)
{
    ui->setupUi(this);
}

AddBuildingBlockDlg::~AddBuildingBlockDlg()
{
    delete ui;
}

void AddBuildingBlockDlg::on_accepted()
{
    const QString name = ui->nameEdit->text();
    const QString ref = ui->refEdit->text();
    emit newBuildingBlock(name, ref);
}
