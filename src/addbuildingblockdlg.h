#ifndef ADDBUILDINGBLOCKDLG_H
#define ADDBUILDINGBLOCKDLG_H

#include <QDialog>

namespace Ui {
class AddBuildingBlockDlg;
}

class AddBuildingBlockDlg : public QDialog
{
    Q_OBJECT

public:
    explicit AddBuildingBlockDlg(QWidget *parent = nullptr);
    ~AddBuildingBlockDlg();

private slots:
    void on_accepted();

signals:
    void newBuildingBlock(const QString& name, const QString& ref);

private:
    Ui::AddBuildingBlockDlg *ui;
};

#endif // ADDBUILDINGBLOCKDLG_H
