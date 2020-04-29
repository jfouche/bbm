#ifndef DLG_BBMANAGER_H
#define DLG_BBMANAGER_H

#include <QDialog>
#include <QSortFilterProxyModel>

class DataModel;
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

class EditableBuildingBlocksModel : public QAbstractListModel
{
    Q_OBJECT

public:
    explicit EditableBuildingBlocksModel(DataModel* datamodel, QObject* parent);

    bool setData(const QModelIndex &index, const QVariant &value, int role = Qt::EditRole) override;
    bool setItemData(const QModelIndex &index, const QMap<int, QVariant> &roles) override;

    int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    int columnCount(const QModelIndex& parent = QModelIndex()) const override;
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;
    Qt::ItemFlags flags(const QModelIndex &index) const override;

    void filter(const QString& filter);
    const QString& filter() const;

//    BuildingBlock* getBuildingBlock(const QModelIndex &index);

private:
    DataModel* m_model;
    QString m_filter;
};

class BuildingBlocksFilterModel : public QSortFilterProxyModel
{
    Q_OBJECT

public:
    BuildingBlocksFilterModel(QObject* parent);

    bool setData(const QModelIndex &index, const QVariant &value, int role = Qt::EditRole) override;
    bool setItemData(const QModelIndex &index, const QMap<int, QVariant> &roles) override;

    void filter(const QString& filter);

protected:
    virtual bool filterAcceptsRow(int source_row, const QModelIndex &source_parent) const override;

private:
    QString m_filter;
};


class BuildingBlockMgrDlg : public QDialog
{
    Q_OBJECT

public:
    explicit BuildingBlockMgrDlg(DataModel* datamodel, QWidget *parent = nullptr);
    ~BuildingBlockMgrDlg();

public slots:
    void updateFilteredBuildingBlocks(const QString& filter);

private:
    Ui::BuildingBlockMgrDlg *ui;
    DataModel* m_model;
    BuildingBlockListModel* m_listBbModel;
    BuildingBlocksCompleterModel* m_BbCompleterModel;
};

#endif // DLG_BBMANAGER_H
