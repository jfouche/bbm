#include "datamodel_file.h"
#include "json_utils.hpp"

static const char* KEY_PROJECT_ARRAY = "projects";
static const char* KEY_PROJECT_NAME = "name";
static const char* KEY_BB_ARRAY = "building_blocks";
static const char* KEY_BB_NAME = "name";
static const char* KEY_BB_REF = "ref";


struct projectIndexValue : public json::key_value
{
    projectIndexValue(const QJsonValue& value)
        : json::key_value("project index", value)
    {}
};

struct bbIndexValue : public json::key_value
{
    bbIndexValue(const QJsonValue& value)
        : json::key_value("bb index", value)
    {}
};

static void read(Project* project, const QJsonObject& jsonObj)
{
    project->setName(json::as<QString>(jsonObj, KEY_PROJECT_NAME));
}

static void read(BuildingBlock* bb, const QJsonObject& jsonObj)
{
    bb->setName(json::as<QString>(jsonObj, KEY_BB_NAME));
    bb->setRef(json::as<QString>(jsonObj, KEY_BB_REF));
}

void read(DataModel& data, const QJsonObject& jsonObj)
{
    for (auto value : json::as<QJsonArray>(jsonObj, KEY_PROJECT_ARRAY)) {
        QJsonObject projObj = json::as<QJsonObject>(projectIndexValue(value));
        auto project = data.addProject();
        read(project, projObj);
    }

    for (auto value : json::as<QJsonArray>(jsonObj, KEY_BB_ARRAY)) {
        QJsonObject projObj = json::as<QJsonObject>(bbIndexValue(value));
        auto bb = data.addBuildingBlock();
        read(bb, projObj);
    }
}

static void write(const Project& project, QJsonObject& obj)
{
    obj[KEY_PROJECT_NAME] = project.name();
}

static void write(const BuildingBlock& bb, QJsonObject& obj)
{
    obj[KEY_BB_NAME] = bb.name();
    obj[KEY_BB_REF] = bb.ref();
}

void write(const DataModel& model, QJsonObject& obj)
{
    QJsonArray projects;
    for (const Project* project : model.projects()) {
        QJsonObject projectObj;
        write(*project, projectObj);
        projects.append(projectObj);
    }
    obj[KEY_PROJECT_ARRAY] = projects;

    QJsonArray buildingblocks;
    for (const BuildingBlock* bb : model.buildingBlocks()) {
        QJsonObject bbObj;
        write(*bb, bbObj);
        projects.append(bbObj);
    }
    obj[KEY_BB_ARRAY] = projects;
}
