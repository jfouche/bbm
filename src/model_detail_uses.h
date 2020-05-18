#ifndef MODEL_USES_H
#define MODEL_USES_H

#include "model_detail.h"

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

class RootUsesTreeItem : public TreeItem
{
public:
    RootUsesTreeItem(Project* project, DetailTreeModel* treeModel);
    RootUsesTreeItem(BuildingBlock* bb, DetailTreeModel* treeModel);

    bool is(void* dataptr) const override;
    virtual QVariant data(int column) const override;
};

#endif // MODEL_USES_H
