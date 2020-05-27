#ifndef MODEL_DETAIL_USEDBY_H
#define MODEL_DETAIL_USEDBY_H

#include "model_detail.h"
#include <QSet>

class UsedByBuildingBlock;

struct UsedByParents {
    QSet<UsedByBuildingBlock*> bb;
    QSet<Project*> project;
};

/**
 * @brief The UsedByBuildingBlock class
 */
class UsedByBuildingBlock : public QObject
{
    Q_OBJECT

public:
    UsedByBuildingBlock(BuildingBlock* bb);

    const BuildingBlock* bb() const;
    const UsedByParents& parents() const;

public slots:
    void addBb(UsedByBuildingBlock* parent);
    void addProject(Project* parent);
    void removeBb(UsedByBuildingBlock* parent);
    void removeProject(Project* parent);

signals:
    void bbAdded(UsedByBuildingBlock*);
    void projectAdded(Project*);
    void bbRemoved(UsedByBuildingBlock*);
    void projectRemoved(Project*);

private:
    const BuildingBlock* m_bb;
    UsedByParents m_parents;
};



/**
 * @brief The UsedByDataModel class
 */
class UsedByDataModel : public QObject
{
    Q_OBJECT

public:
    UsedByDataModel(DataModel* model);

    UsedByBuildingBlock* get(BuildingBlock* bb);

private:
    void insert(BuildingBlock* bb, BuildingBlock* parent);
    void insert(BuildingBlock* bb, Project* parent);
    void remove(BuildingBlock* bb, BuildingBlock* parent);
    void remove(BuildingBlock* bb, Project* parent);

private:
    QVector<UsedByBuildingBlock*> m_listBb;
};

/**
 * @brief The ProjectUsedByTreeItem class
 */
class ProjectUsedByTreeItem : public TreeItem
{
    Q_OBJECT

public:
    ProjectUsedByTreeItem(Project* project, TreeItem* parent);

    bool is(void* dataptr) const override;
    QVariant data(int column) const override;

private:
    Project* m_project;
};

/**
 * @brief The BuildingBlockUsedByTreeItem class
 */
class BuildingBlockUsedByTreeItem : public TreeItem
{
    Q_OBJECT

public:
    BuildingBlockUsedByTreeItem(UsedByBuildingBlock* bb, TreeItem* parent);

    bool is(void* dataptr) const override;
    QVariant data(int column) const override;

public slots:
    void addBb(UsedByBuildingBlock* bb);
    void addProject(Project* project);
    void removeBb(UsedByBuildingBlock* bb);
    void removeProject(Project* project);

private:
    UsedByBuildingBlock* m_bb;
};

/**
 * @brief The RootUsedByTreeItem class
 */
class RootUsedByTreeItem : public TreeItem
{
public:
    RootUsedByTreeItem(Project* project, DetailTreeModel* treeModel);
    RootUsedByTreeItem(UsedByBuildingBlock* bb, DetailTreeModel* treeModel);

    bool is(void* dataptr) const override;
    QVariant data(int column) const override;
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

private:
    void watchBuildingBlock(BuildingBlock* bb);
    void addBuildingBlock(Project* parent, BuildingBlock* bb);
    void addBuildingBlock(BuildingBlock* parent, BuildingBlock* bb);

private:
    UsedByDataModel m_reverseModel;
};

#endif // MODEL_DETAIL_USEDBY_H
