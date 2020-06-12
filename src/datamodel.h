#ifndef DATABASE_H
#define DATABASE_H

#include <QObject>
#include <QVector>
#include <QList>

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

    const QList<BuildingBlock*>& buildingBlocks() const;

    void add(BuildingBlock* bb);
    void remove(BuildingBlock* bb);
    bool contains(BuildingBlock* bb) const;

signals:
    void changed();
    void bbAdded(BuildingBlock* child);
    void bbRemoved(BuildingBlock* child);

private:
    QString m_name;
    QList<BuildingBlock*> m_listBb;
};

/**
 * @brief The BuildingBlock struct
 */
struct BuildingBlock : public QObject
{
    Q_OBJECT

public:
    BuildingBlock(QObject* parent);

    enum Maturity {
        A, B, C, D, E, F
    };

    const QString& name() const { return m_name; }
    void setName(const QString& name);

    const QString& ref() const { return m_ref; }
    void setRef(const QString& ref);

    const QString& info() const { return m_info; }
    void setInfo(const QString& name);

    Maturity maturity() const { return m_maturity; }
    void setMaturity(Maturity maturity);

    void add(BuildingBlock* bb);
    void remove(BuildingBlock* bb);
    const QList<BuildingBlock*>& children();
    const QList<BuildingBlock*>& children() const;

    const QList<BuildingBlock*>& parentBb();
    const QList<BuildingBlock*>& parentBb() const;
    const QList<Project*>& parentProject();
    const QList<Project*>& parentProject() const;

    void addParent(Project* project);
    void addParent(BuildingBlock* parent);
    void removeParent(Project* project);
    void removeParent(BuildingBlock* parent);

    /**
    * @brief contains
    * @param bb
    * @return true if the BB or its children contains the BB
    */
    bool contains(BuildingBlock* bb) const;

signals:
    void changed();
    void childAdded(BuildingBlock* child);
    void childRemoved(BuildingBlock* child);
    void parentAdded(BuildingBlock* parent);
    void parentAdded(Project* parent);
    void parentRemoved(BuildingBlock* parent);
    void parentRemoved(Project* parent);

private:
    QString m_name;
    QString m_ref;
    Maturity m_maturity;
    QString m_info;
    QList<BuildingBlock*> m_children;
    QList<BuildingBlock*> m_parentBb;
    QList<Project*> m_parentProjects;
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

    Project* addProject();
    const Projects& projects() const;
    void deleteProject(Project* project);

    BuildingBlock* addBuildingBlock();
    const BuildingBlocks& buildingBlocks() const;
    bool deleteBuildingBlock(BuildingBlock* bb);

    void save(const QString& path) const;
    void load(const QString& path);
    void clear();

signals:
    void projectAdded(Project*);
    void projectDeleting(Project*);
    void buildingBlockAdded(BuildingBlock*);
    void buildingBlockDeleting(BuildingBlock*);
    void cleared();

private:
    Projects m_projects;
    BuildingBlocks m_buildingblocks;
};

#endif // DATABASE_H
