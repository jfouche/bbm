#ifndef DLG_BBMANAGER_H
#define DLG_BBMANAGER_H

#include <QDialog>
#include <QSortFilterProxyModel>

class DataModel;
class BuildingBlock;
class BuildingBlockListModel;

namespace Ui {
class BuildingBlockMgrDlg;
}


class BuildingBlocksCompleterModel : public QAbstractListModel
{
    Q_OBJECT

public:
    explicit BuildingBlocksCompleterModel(DataModel* datamodel, QObject* parent);

    int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    int columnCount(const QModelIndex& parent = QModelIndex()) const override;
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;

    void filter(const QString& filter);
    const QString& filter() const;

private:
    DataModel* m_model;
    QString m_filter;
};

class AvailableBuildingBlockChildrenModel : public QSortFilterProxyModel
{
    Q_OBJECT

public:
    explicit AvailableBuildingBlockChildrenModel(BuildingBlockListModel* model, QObject* parent);
    explicit AvailableBuildingBlockChildrenModel(DataModel* datamodel, QObject* parent);

    bool filterAcceptsRow(int sourceRow, const QModelIndex &sourceParent) const override;
    Qt::ItemFlags flags(const QModelIndex &index) const override;
    QVariant data(const QModelIndex &index, int role) const override;
    bool setData(const QModelIndex &index, const QVariant &value, int role) override;

    void setParentBuildingBlock(BuildingBlock* parentBb);

private:
    BuildingBlockListModel* m_model;
    BuildingBlock* m_parentBB;
};

class BuildingBlockMgrDlg : public QDialog
{
    Q_OBJECT

public:
    explicit BuildingBlockMgrDlg(DataModel* datamodel, QWidget *parent = nullptr);
    ~BuildingBlockMgrDlg();

public slots:
    void updateBuildingBlockChildren();

private:
    Ui::BuildingBlockMgrDlg *ui;
    DataModel* m_model;
    AvailableBuildingBlockChildrenModel* m_childrenBbModel;
    BuildingBlockListModel* m_BbListModel;
    BuildingBlocksCompleterModel* m_BbCompleterModel;
};

#endif // DLG_BBMANAGER_H
