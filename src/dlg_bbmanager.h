#ifndef DLG_BBMANAGER_H
#define DLG_BBMANAGER_H

#include <QDialog>
#include <QSortFilterProxyModel>

class DataModel;
class BuildingBlock;
class BuildingBlockListModel;
class BuildingBlockTreeModel;
class AvailableBuildingBlockChildrenModel;

namespace Ui {
class BuildingBlockMgrDlg;
}

/**
 * @brief The BuildingBlocksCompleterModel class
 */
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

/**
 * @brief The CurrentBbTreeModel class
 */
class CurrentBbTreeModel : public QSortFilterProxyModel
{
    Q_OBJECT

public:
    CurrentBbTreeModel(BuildingBlockTreeModel* model, QObject* parent);
    CurrentBbTreeModel(DataModel* datamodel, QObject* parent);

    bool filterAcceptsRow(int sourceRow, const QModelIndex &sourceParent) const override;
//    Qt::ItemFlags flags(const QModelIndex &index) const override;

    void setParentBuildingBlock(BuildingBlock* parentBb);

private:
    BuildingBlockTreeModel* m_model;
    BuildingBlock* m_parentBB;
};



/**
 * @brief The BuildingBlockMgrDlg class
 */
class BuildingBlockMgrDlg : public QDialog
{
    Q_OBJECT

public:
    explicit BuildingBlockMgrDlg(DataModel* datamodel, QWidget *parent = nullptr);
    ~BuildingBlockMgrDlg();

public slots:
    void setCurrentBuildingBlock(int index);

private:
    Ui::BuildingBlockMgrDlg *ui;
    DataModel* m_model;
    AvailableBuildingBlockChildrenModel* m_childrenBbModel;
    BuildingBlockListModel* m_BbListModel;
    BuildingBlocksCompleterModel* m_BbCompleterModel;
    CurrentBbTreeModel* m_currentBbTreeModel;
};

#endif // DLG_BBMANAGER_H
