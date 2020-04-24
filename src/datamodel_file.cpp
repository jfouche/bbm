#include "datamodel_file.h"
#include "json_utils.hpp"

void read(DataModel& data, const QJsonObject& jsonObj)
{
    auto value = json::check_exists(jsonObj, "projects");
    const QJsonArray array = json::check_type<QJsonArray>(value);
    for (auto it = array.begin(); it != array.end(); ++it) {
        QJsonObject projObj = json::check_type<QJsonObject>(json::json_pair("project index", *it));
        auto project =  data.addProject();
        project->setName(json::read(projObj, "name"));
    }
    if (jsonObj.contains("projects") && jsonObj["projects"].isObject()) {

    }
}

/*
static void read(Project& project, const QJsonObject& jsonObj)
{
    std::function<void(QString)> cb = [&project](QString value) {
        project.name = value;
    };
    project.name = json::read(jsonObj, "name");
}
*/

static void write(const Project& project, QJsonObject& obj)
{
    obj["name"] = project.name();
}

static void write(const BuildingBlock& bb, QJsonObject& obj)
{
    obj["name"] = bb.name();
    obj["ref"] = bb.ref();
}

void write(const DataModel& model, QJsonObject& obj)
{
    QJsonArray projects;
    for (int i = 0; i < model.getProjectCount(); ++i) {
        QJsonObject projectObj;
        const Project* project = model.getProject(i);
        if (project) {
            write(*project, projectObj);
            projects.append(projectObj);
        }
    }
    obj["projects"] = projects;

    QJsonArray buildingblocks;
    for (int i = 0; i < model.getBuildingBlockCount(); ++i) {
        QJsonObject bbObj;
        const BuildingBlock* bb = model.getBuildingBlock(i);
        if (bb) {
            write(*bb, bbObj);
            projects.append(bbObj);
        }
    }
    obj["buildingblocks"] = projects;
}
