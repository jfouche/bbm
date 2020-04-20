#ifndef DATABASE_H
#define DATABASE_H

#include <QObject>
#include <QList>

struct Project {
    QString name;

    Project(const QString& name)
        : name(name)
    {
    }
};

struct BuildingBlock {
    QString name;
    QString ref;
    QString info;
    QList<BuildingBlock> children;
};

class DataModel : public QObject
{
    QList<Project> m_projects;
    QList<BuildingBlock> m_buildingblocks;

    Q_OBJECT
public:
    explicit DataModel(QObject *parent = nullptr);

    bool addProject(const QString& name);

    int getProjectCount() const;

    const Project& getProject(int index) const;

signals:
    void dbChanged();
};

#endif // DATABASE_H
