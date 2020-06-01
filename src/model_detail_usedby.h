#ifndef MODEL_DETAIL_USEDBY_H
#define MODEL_DETAIL_USEDBY_H

#include "model_detail.h"
#include <QSet>


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
    BuildingBlockUsedByTreeItem(BuildingBlock* bb, TreeItem* parent);

    bool is(void* dataptr) const override;
    QVariant data(int column) const override;

private:
    BuildingBlock* m_bb;
};

/**
 * @brief The RootUsedByTreeItem class
 */
class RootUsedByTreeItem : public TreeItem
{
public:
    RootUsedByTreeItem(Project* project, DetailTreeModel* treeModel);
    RootUsedByTreeItem(BuildingBlock* bb, DetailTreeModel* treeModel);

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
};

#endif // MODEL_DETAIL_USEDBY_H
