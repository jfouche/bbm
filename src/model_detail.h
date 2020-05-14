#ifndef MODEL_DETAIL_H
#define MODEL_DETAIL_H

#include <QAbstractItemModel>

class DataModel;
class Project;
class BuildingBlock;
class TreeItem;
class DetailTreeModel;

/**
 * @brief The TreeItem class
 */
class TreeItem : public QObject
{
public:
    explicit TreeItem(DetailTreeModel* treeModel);
    explicit TreeItem(TreeItem* parent);
    ~TreeItem();

    void appendChild(TreeItem *child);
    TreeItem *child(int row);
    int childCount() const;
    int row() const;
    TreeItem *parentItem();

    virtual QVariant data(int column) const = 0;
    virtual bool is(void* dataptr) const = 0;

protected:
//    void add(BuildingBlock* bb);
    void remove(void* dataptr);
    void update();

protected:
    QModelIndex index(int col = 0);
    void add(TreeItem* item, const QModelIndex& parentIdx, int pos);
    const QList<TreeItem*>& children() const;

private:
    QList<TreeItem*> m_children;
    TreeItem *m_parent;
    DetailTreeModel* m_treeModel;
};

namespace uses {

class ProjectTreeItem : public TreeItem
{
    Q_OBJECT

public:
    ProjectTreeItem(Project* project, TreeItem* parent);

    bool is(void* dataptr) const override;
    virtual QVariant data(int column) const override;

private slots:
    void add(BuildingBlock* bb);
    void remove(BuildingBlock* bb);

private:
    Project* m_project;
};

class BuildingBlockTreeItem : public TreeItem
{
    Q_OBJECT

public:
    BuildingBlockTreeItem(BuildingBlock* bb, TreeItem* parent);

    bool is(void* dataptr) const override;
    virtual QVariant data(int column) const override;

private slots:
    void add(BuildingBlock* bb);
    void remove(BuildingBlock* bb);

private:
    BuildingBlock* m_bb;
};

class RootTreeItem : public TreeItem
{
public:
    RootTreeItem(Project* project, DetailTreeModel* treeModel);
    RootTreeItem(BuildingBlock* bb, DetailTreeModel* treeModel);

    bool is(void* dataptr) const override;
    virtual QVariant data(int column) const override;
};

} // namespace uses

/**
 * @brief The DetailTreeModel class
 */
class DetailTreeModel : public QAbstractItemModel
{
    Q_OBJECT

    friend class TreeItem;

public:
    explicit DetailTreeModel(DataModel* model, QObject *parent = nullptr);
    ~DetailTreeModel();

    QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const override;
    QModelIndex index(int row, int column, const QModelIndex &parent = QModelIndex()) const override;
    QModelIndex parent(const QModelIndex &index) const override;
    int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    int columnCount(const QModelIndex &parent = QModelIndex()) const override;
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;

    TreeItem* treeItem(const QModelIndex& index) const;
    TreeItem* treeItem(int row, const QModelIndex& parent);

    void set(Project* project);
    void set(BuildingBlock* bb);

protected:
    DataModel* m_model;
    TreeItem* m_rootItem;
};


/**
 * @brief The UsesTreeModel class
 */
class UsesTreeModel : public DetailTreeModel
{
    Q_OBJECT

public:
    explicit UsesTreeModel(DataModel* model, QObject *parent = nullptr);

    void set(Project* project);
    void set(BuildingBlock* bb);
};

/**
 * @brief The UsedByTreeModel class
 */
class UsedByTreeModel : public DetailTreeModel
{
    Q_OBJECT

public:
    explicit UsedByTreeModel(DataModel* model, QObject *parent = nullptr);

    void set(Project* project);
    void set(BuildingBlock* bb);
};

#endif // MODEL_DETAIL_H
