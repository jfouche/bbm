#ifndef MODEL_DETAIL_USEDBY_H
#define MODEL_DETAIL_USEDBY_H

#include "model_detail.h"
#include <QSet>

/**
 * @brief The UsedByDataModel class
 */
class UsedByDataModel
{
private:
    struct Parents {
        QSet<BuildingBlock*> bb;
        QSet<Project*> project;
    };

public:
    typedef QMap<BuildingBlock*, Parents> MapParents;
    typedef MapParents::const_iterator const_iterator;

    const const_iterator operator()(BuildingBlock* bb) const {
        return parents.find(bb);
    }

    const_iterator end() const {
        return parents.end();
    }

    void insert(BuildingBlock* bb, BuildingBlock* parent) {
        parents[bb].bb.insert(parent);
    }

    void insert(BuildingBlock* bb, Project* parent) {
        parents[bb].project.insert(parent);
    }

    void remove(BuildingBlock* bb, BuildingBlock* parent) {
        parents[bb].bb.remove(parent);
    }

    void remove(BuildingBlock* bb, Project* parent) {
        parents[bb].project.remove(parent);
    }

private:
    MapParents parents;
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
    virtual QVariant data(int column) const override;

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
    BuildingBlockUsedByTreeItem(BuildingBlock* bb, const UsedByDataModel& parents, TreeItem* parent);

    bool is(void* dataptr) const override;
    virtual QVariant data(int column) const override;

private:
    BuildingBlock* m_bb;
    const UsedByDataModel& m_model;
};

/**
 * @brief The RootUsedByTreeItem class
 */
class RootUsedByTreeItem : public TreeItem
{
public:
    RootUsedByTreeItem(Project* project, const UsedByDataModel& model, DetailTreeModel* treeModel);
    RootUsedByTreeItem(BuildingBlock* bb, const UsedByDataModel& model, DetailTreeModel* treeModel);

    bool is(void* dataptr) const override;
    virtual QVariant data(int column) const override;

private:
    const UsedByDataModel& m_model;
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
