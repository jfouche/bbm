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
    Project(const QString& name, QObject* parent)
        : QObject(parent)
        , m_name(name)
    {
    }

    const QString& name() const { return m_name; }

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
    BuildingBlock(const QString& name, const QString& ref, QObject* parent);

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

    Project* addProject(const QString& name);
    BuildingBlock* addBuildingBlock(const QString& name, const QString& ref);

    int getProjectCount() const;

    const Project* getProject(int index) const;
    const Project* getProject(const QString& name) const;

    const BuildingBlock* getBuildingBlock(const QString& ref) const;

    void save(const QString& path) const;
    void load(const QString& path);

private slots:
    void changed();

signals:
    void dbChanged();
};

#endif // DATABASE_H
