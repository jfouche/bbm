#include "dlg_buildingblock.h"
#include "ui_dlg_buildingblock.h"
#include <QStringListModel>

BuildingBlockEditDlg::BuildingBlockEditDlg(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::AddBuildingBlockDlg)
{
    ui->setupUi(this);

    QStringListModel *model = new QStringListModel();
    QStringList list = {"A", "B", "C", "D", "E", "F"};
    model->setStringList(list);
    ui->comboMaturity->setModel(model);
}

BuildingBlockEditDlg::~BuildingBlockEditDlg()
{
    delete ui;
}

void BuildingBlockEditDlg::set(const BuildingBlock &bb)
{
    ui->nameEdit->setText(bb.name());
    ui->refEdit->setText(bb.ref());
    ui->infoEdit->setText(bb.info());
    ui->comboMaturity->setCurrentIndex(bb.maturity());
}

QString BuildingBlockEditDlg::getName() const
{
    return ui->nameEdit->text();
}

QString BuildingBlockEditDlg::getRef() const
{
    return ui->refEdit->text();
}

QString BuildingBlockEditDlg::getInfo() const
{
    return ui->infoEdit->text();
}

BuildingBlock::Maturity BuildingBlockEditDlg::getMaturity() const
{
    return static_cast<BuildingBlock::Maturity>(ui->comboMaturity->currentIndex());
}

