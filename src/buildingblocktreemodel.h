#ifndef BUILDINGBLOCKTREEMODEL_H
#define BUILDINGBLOCKTREEMODEL_H

#include <QAbstractItemModel>

class DataModel;
class BuildingBlock;
class TreeItem;
class BuildingBlockTreeModel;

class TreeItem : public QObject
{
    Q_OBJECT
public:
    explicit TreeItem(DataModel* bb, BuildingBlockTreeModel* treeModel);
    explicit TreeItem(BuildingBlock* bb, TreeItem* parent, BuildingBlockTreeModel* treeModel);
    ~TreeItem();

    void appendChild(TreeItem *child);
    QVariant data(int column) const;
    TreeItem *child(int row);
    int childCount() const;
    BuildingBlock* data() const;
    int row() const;
    TreeItem *parentItem();

private slots:
    void bbAdded(BuildingBlock* bb);

private:
    QVector<TreeItem*> m_children;
    TreeItem *m_parent;
    BuildingBlockTreeModel* m_treeModel;
    BuildingBlock* m_bb;
};

class BuildingBlockTreeModel : public QAbstractItemModel
{
    Q_OBJECT

    friend class TreeItem;

public:
    explicit BuildingBlockTreeModel(DataModel* model, QObject *parent = nullptr);
    ~BuildingBlockTreeModel();

    QModelIndex index(int row, int column, const QModelIndex &parent = QModelIndex()) const override;
    QModelIndex parent(const QModelIndex &index) const override;
    int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    int columnCount(const QModelIndex &parent = QModelIndex()) const override;
    QVariant data(const QModelIndex &index, int role) const override;

    Qt::ItemFlags flags(const QModelIndex &index) const override;
    QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const override;

private slots:
    void update();

private:
    void init();

    DataModel* m_model;
    TreeItem* m_rootItem;
};

#endif // BUILDINGBLOCKTREEMODEL_H
