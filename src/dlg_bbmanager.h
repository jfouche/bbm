#ifndef DLG_BBMANAGER_H
#define DLG_BBMANAGER_H

#include <QDialog>

class DataModel;
class BuildingBlockListModel;

namespace Ui {
class BuildingBlockMgrDlg;
}

class BuildingBlockMgrDlg : public QDialog
{
    Q_OBJECT

public:
    explicit BuildingBlockMgrDlg(DataModel* datamodel, QWidget *parent = nullptr);
    ~BuildingBlockMgrDlg();

private:
    Ui::BuildingBlockMgrDlg *ui;
    DataModel* m_model;
    BuildingBlockListModel* m_listBbModel;
};

#endif // DLG_BBMANAGER_H
