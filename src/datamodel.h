#ifndef DATABASE_H
#define DATABASE_H

#include <QObject>
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
    QList<Project*> m_projects;
    QList<BuildingBlock*> m_buildingblocks;

    Q_OBJECT
public:
    explicit DataModel(QObject *parent = nullptr);

    Project* addProject();
    BuildingBlock* addBuildingBlock();

    int getProjectCount() const;
    int getBuildingBlockCount() const;

    const Project* getProject(int index) const;
    Project* getProject(int index);

    const BuildingBlock* getBuildingBlock(int index) const;
    BuildingBlock* getBuildingBlock(int index);

    void save(const QString& path) const;
    void load(const QString& path);

private slots:
    void changed();

signals:
    void dbChanged();
};

#endif // DATABASE_H
