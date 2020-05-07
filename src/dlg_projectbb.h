#ifndef DLG_PROJECTBB_H
#define DLG_PROJECTBB_H

#include <QDialog>

class DataModel;
class ProjectListModel;

namespace Ui {
class ProjectBuildingBlocksDlg;
}

class ProjectBuildingBlocksDlg : public QDialog
{
    Q_OBJECT

public:
    explicit ProjectBuildingBlocksDlg(DataModel* model, QWidget *parent = nullptr);
    ~ProjectBuildingBlocksDlg();

private:
    Ui::ProjectBuildingBlocksDlg *ui;
    DataModel* m_model;
    ProjectListModel* m_projectListModel;
};

#endif // DLG_PROJECTBB_H
