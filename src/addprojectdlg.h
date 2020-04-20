#ifndef ADDPROJECTDLG_H
#define ADDPROJECTDLG_H

#include <QDialog>

QT_BEGIN_NAMESPACE
namespace Ui { class AddProjectDlg; }
QT_END_NAMESPACE

class AddProjectDlg : public QDialog
{
    Q_OBJECT

public:
    AddProjectDlg(QWidget *parent = nullptr);
    ~AddProjectDlg();

private slots:
    void on_buttonBox_accepted();

signals:
    void newProjectName(const QString& name);

private:
    Ui::AddProjectDlg *ui;
};

#endif // ADDPROJECTDLG_H
