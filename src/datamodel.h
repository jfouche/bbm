#ifndef DATABASE_H
#define DATABASE_H

#include <QObject>
#include <QList>

struct Project;
struct BuildingBlock;

/**
 * @brief The Project struct
 */
struct Project {
    QString name;
    //QList<BuildingBlock const * const> children;

    Project(const QString& name)
        : name(name)
    {
    }

};

/**
 * @brief The BuildingBlock struct
 */
struct BuildingBlock {
    QString name;
    QString ref;
    QString info;
    //QList<BuildingBlock const * const> children;

    BuildingBlock(const QString& name, const QString& ref)
        : name(name)
        , ref(ref)
    {
    }
};

/**
 * @brief The DataModel class
 */
class DataModel : public QObject
{
    QList<Project> m_projects;
    QList<BuildingBlock> m_buildingblocks;

    Q_OBJECT
public:
    explicit DataModel(QObject *parent = nullptr);

    const Project* addProject(const QString& name);
    const BuildingBlock* addBuildingBlock(const QString& name, const QString& ref);

    int getProjectCount() const;

    const Project* getProject(int index) const;
    const Project* getProject(const QString& name) const;

    const BuildingBlock* getBuildingBlock(const QString& ref) const;

    void save(const QString& path) const;
    void load(const QString& path);

signals:
    void dbChanged();
};

#endif // DATABASE_H
