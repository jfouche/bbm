#ifndef ADDBUILDINGBLOCKDLG_H
#define ADDBUILDINGBLOCKDLG_H

#include <QDialog>
#include "datamodel.h"

namespace Ui {
class AddBuildingBlockDlg;
}

class BuildingBlockEditDlg : public QDialog
{
    Q_OBJECT

public:
    explicit BuildingBlockEditDlg(QWidget *parent = nullptr);
    ~BuildingBlockEditDlg();

    void set(const BuildingBlock& bb);
    QString getName() const;
    QString getRef() const;

private:
    Ui::AddBuildingBlockDlg *ui;
};

#endif // ADDBUILDINGBLOCKDLG_H
