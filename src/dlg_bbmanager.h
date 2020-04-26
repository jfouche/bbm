#ifndef DLG_BBMANAGER_H
#define DLG_BBMANAGER_H

#include <QAbstractListModel>
#include <QDialog>

class DataModel;

namespace Ui {
class BuildingBlockMgrDlg;
}


class BuildingBlockListModel : public QAbstractListModel
{
    Q_OBJECT
public:
    explicit BuildingBlockListModel(DataModel* datamodel, QObject* parent);

    int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;

private slots:
    void update();

private:
    DataModel* m_datamodel;
};

class BuildingBlockMgrDlg : public QDialog
{
    Q_OBJECT

public:
    explicit BuildingBlockMgrDlg(DataModel* datamodel, QWidget *parent = nullptr);
    ~BuildingBlockMgrDlg();

private:
    Ui::BuildingBlockMgrDlg *ui;
    DataModel* m_datamodel;
    BuildingBlockListModel* m_listBbModel;
};

#endif // DLG_BBMANAGER_H
