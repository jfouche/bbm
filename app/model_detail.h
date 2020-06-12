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

    enum Columns {
        COL_NAME,
        COL_REF,
        COL_MATURITY,
        COL_INFO,
        COL_COUNT
    };

    static const std::vector<QString> MaturityLabel;

    TreeItem *child(int row);
    int childCount() const;
    int row() const;
    TreeItem *parentItem();

    virtual QVariant data(int column) const = 0;
    virtual bool is(void* dataptr) const = 0;

protected:
    void appendChild(TreeItem *child);
    void remove(void* dataptr);
    void update();
    QModelIndex index(int col = 0);
    void add(TreeItem* item);
    const QList<TreeItem*>& children() const;

private:
    QList<TreeItem*> m_children;
    TreeItem *m_parent;
    DetailTreeModel* m_treeModel;
};


/**
 * @brief The DetailTreeModel class
 */
class DetailTreeModel : public QAbstractItemModel
{
    Q_OBJECT

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
    QModelIndex index(TreeItem* item, int col = 0) const;
    void childAdded(TreeItem* item, int pos);
    void childRemoved(TreeItem* item, int pos);
    void update(TreeItem* item);

    void set(Project* project);
    void set(BuildingBlock* bb);

private slots:
    void clear();

protected:
    DataModel* m_model;
    TreeItem* m_rootItem;
};

#endif // MODEL_DETAIL_H
