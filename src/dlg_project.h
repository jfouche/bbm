#ifndef ADDPROJECTDLG_H
#define ADDPROJECTDLG_H

#include <QDialog>
#include "datamodel.h"

QT_BEGIN_NAMESPACE
namespace Ui { class ProjectDlg; }
QT_END_NAMESPACE

class ProjectEditDlg : public QDialog
{
    Q_OBJECT

public:
    ProjectEditDlg(QWidget *parent = nullptr);
    ~ProjectEditDlg();

    void setProject(const Project& project);
    QString getName() const;

private:
    Ui::ProjectDlg *ui;
};

#endif // ADDPROJECTDLG_H
