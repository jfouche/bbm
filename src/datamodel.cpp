#include "datamodel.h"

DataModel::DataModel(QObject *parent) : QObject(parent)
{
    m_projects.push_back(Project("Project1"));
    m_projects.push_back(Project("Project2"));
    m_projects.push_back(Project("Project3"));
    m_projects.push_back(Project("Project4"));
}

bool DataModel::addProject(const QString &name)
{
    auto by_name = [name](const Project& p) {
        return p.name == name;
    };
    if (std::find_if(m_projects.begin(), m_projects.end(), by_name) != m_projects.end()) {
        return false;
    }
    m_projects.push_back(Project(name));
    emit dbChanged();
    return true;
}

int DataModel::getProjectCount() const
{
    return m_projects.size();
}

const Project& DataModel::getProject(int index) const
{
    return m_projects.at(index);
}
