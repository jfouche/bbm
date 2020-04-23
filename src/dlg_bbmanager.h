#ifndef DLG_BBMANAGER_H
#define DLG_BBMANAGER_H

#include <QDialog>

namespace Ui {
class BuildingBlockMgrDlg;
}

class BuildingBlockMgrDlg : public QDialog
{
    Q_OBJECT

public:
    explicit BuildingBlockMgrDlg(QWidget *parent = nullptr);
    ~BuildingBlockMgrDlg();

private:
    Ui::BuildingBlockMgrDlg *ui;
};

#endif // DLG_BBMANAGER_H
