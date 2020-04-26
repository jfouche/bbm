#ifndef DATABASE_H
#define DATABASE_H

#include <QObject>
#include <QVector>
#include "ctn_view.hpp"

struct Project;
struct BuildingBlock;

/**
 * @brief The Project struct
 */
struct Project : public QObject
{
    Q_OBJECT

public:
    Project(QObject* parent);

    const QString& name() const { return m_name; }
    void setName(const QString& name);

signals:
    void changed();

private:
    QString m_name;
};

/**
 * @brief The BuildingBlock struct
 */
struct BuildingBlock : public QObject
{
    Q_OBJECT

public:
    BuildingBlock(QObject* parent);

    const QString& name() const { return m_name; }
    void setName(const QString& name);

    const QString& ref() const { return m_ref; }
    void setRef(const QString& ref);

signals:
    void changed();

private:
    QString m_name;
    QString m_ref;
    QString m_info;
    //QList<BuildingBlock const * const> children;
};

/**
 * @brief The DataModel class
 */
class DataModel : public QObject
{
    Q_OBJECT

public:
    explicit DataModel(QObject *parent = nullptr);

    typedef QVector<Project*> Projects;
    typedef QVector<BuildingBlock*> BuildingBlocks;

    typedef view<Projects, Project*> ProjectsView;
    typedef view<BuildingBlocks, BuildingBlock*> BuildingBlocksView;


    Project* addProject();
    BuildingBlock* addBuildingBlock();

    ProjectsView projects() const;
    const Projects& projects();

    BuildingBlocksView buildingBlocks() const;
    const BuildingBlocks& buildingBlocks();

    void save(const QString& path) const;
    void load(const QString& path);

private slots:
    void changed();

signals:
    void modelChanged();
    void projectAdded(Project*);
    void buildingBlockAdded(BuildingBlock*);

private:
    Projects m_projects;
    BuildingBlocks m_buildingblocks;
};

#endif // DATABASE_H
