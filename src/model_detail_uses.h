#ifndef MODEL_USES_H
#define MODEL_USES_H

#include "model_detail.h"

/**
 * @brief The ProjectUsesTreeItem class
 */
class ProjectUsesTreeItem : public TreeItem
{
    Q_OBJECT

public:
    ProjectUsesTreeItem(Project* project, TreeItem* parent);

    bool is(void* dataptr) const override;
    virtual QVariant data(int column) const override;

private slots:
    void add(BuildingBlock* bb);
    void remove(BuildingBlock* bb);

private:
    Project* m_project;
};

/**
 * @brief The BuildingBlockUsesTreeItem class
 */
class BuildingBlockUsesTreeItem : public TreeItem
{
    Q_OBJECT

public:
    BuildingBlockUsesTreeItem(BuildingBlock* bb, TreeItem* parent);

    bool is(void* dataptr) const override;
    virtual QVariant data(int column) const override;

private slots:
    void add(BuildingBlock* bb);
    void remove(BuildingBlock* bb);

private:
    BuildingBlock* m_bb;
};

/**
 * @brief The RootUsesTreeItem class
 */
class RootUsesTreeItem : public TreeItem
{
public:
    RootUsesTreeItem(Project* project, DetailTreeModel* treeModel);
    RootUsesTreeItem(BuildingBlock* bb, DetailTreeModel* treeModel);

    bool is(void* dataptr) const override;
    virtual QVariant data(int column) const override;
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

#endif // MODEL_USES_H
