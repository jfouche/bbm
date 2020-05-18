#ifndef MODEL_DETAIL_USEDBY_H
#define MODEL_DETAIL_USEDBY_H

#include "model_detail.h"

class ProjectUsedByTreeItem : public TreeItem
{
    Q_OBJECT

public:
    ProjectUsedByTreeItem(Project* project, TreeItem* parent);

    bool is(void* dataptr) const override;
    virtual QVariant data(int column) const override;

private slots:
    void add(BuildingBlock* bb);
    void remove(BuildingBlock* bb);

private:
    Project* m_project;
};

class BuildingBlockUsedByTreeItem : public TreeItem
{
    Q_OBJECT

public:
    BuildingBlockUsedByTreeItem(BuildingBlock* bb, TreeItem* parent);

    bool is(void* dataptr) const override;
    virtual QVariant data(int column) const override;

private slots:
    void add(BuildingBlock* bb);
    void remove(BuildingBlock* bb);

private:
    BuildingBlock* m_bb;
};

class RootUsedByTreeItem : public TreeItem
{
public:
    RootUsedByTreeItem(Project* project, DetailTreeModel* treeModel);
    RootUsedByTreeItem(BuildingBlock* bb, DetailTreeModel* treeModel);

    bool is(void* dataptr) const override;
    virtual QVariant data(int column) const override;
};

#endif // MODEL_DETAIL_USEDBY_H
