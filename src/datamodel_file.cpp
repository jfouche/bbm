#include "datamodel_file.h"
#include "json_utils.hpp"
#include <QHash>


static const char* KEY_PROJECT_ARRAY = "projects";
static const char* KEY_PROJECT_NAME = "name";
static const char* KEY_BB_ARRAY = "building_blocks";
static const char* KEY_BB_NAME = "name";
static const char* KEY_BB_REF = "ref";
static const char* KEY_BB_ID = "id";

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


class JsonWriter
{
public:
    JsonWriter(const DataModel& model);

    void write(QJsonObject& jsonObj) const;

private:
    void write(const Project* project, QJsonObject& jsonObj) const;
    void write(const BuildingBlock* project, QJsonObject& jsonObj) const;

private:
    const DataModel& m_model;
    QHash<const BuildingBlock*, int> m_bbHash;
};

class JsonReader
{
public:
    JsonReader(DataModel& model);

    void read(const QJsonObject& jsonObj);

private:
    void read(Project* project, const QJsonObject& jsonObj);
    void read(BuildingBlock* project, const QJsonObject& jsonObj);

private:
    DataModel& m_model;
    QHash<int, BuildingBlock*> m_bbHash;
};

JsonReader::JsonReader(DataModel& model)
    : m_model(model)
{
}


void JsonReader::read(Project* project, const QJsonObject& jsonObj)
{
    project->setName(json::as<QString>(jsonObj, KEY_PROJECT_NAME));
}

void JsonReader::read(BuildingBlock* bb, const QJsonObject& jsonObj)
{
    bb->setName(json::as<QString>(jsonObj, KEY_BB_NAME));
    bb->setRef(json::as<QString>(jsonObj, KEY_BB_REF));
    int id = json::as<int>(jsonObj, KEY_BB_ID);
    m_bbHash[id] = bb;
}

void JsonReader::read(const QJsonObject& jsonObj)
{
    // Read projects
    for (auto value : json::as<QJsonArray>(jsonObj, KEY_PROJECT_ARRAY)) {
        QJsonObject projObj = json::as<QJsonObject>(projectIndexValue(value));
        auto project = m_model.addProject();
        read(project, projObj);
    }

    // read BB
    QHash<int, BuildingBlock*> bbIds;
    for (auto value : json::as<QJsonArray>(jsonObj, KEY_BB_ARRAY)) {
        QJsonObject bbObj = json::as<QJsonObject>(bbIndexValue(value));
        auto bb = m_model.addBuildingBlock();
        read(bb, bbObj);
        int id = json::as<int>(bbObj, KEY_BB_ID);
        bbIds[id] = bb;
    }

    // link BB children to BB
    /// TODO
    // link BB children to projects
    /// TODO
}

JsonWriter::JsonWriter(const DataModel& model)
    : m_model(model)
{
    int id = 1;
    for (const BuildingBlock* bb : m_model.buildingBlocks()) {
        m_bbHash[bb] = id++;
    }
}

void JsonWriter::write(const Project* project, QJsonObject& obj) const
{
    obj[KEY_PROJECT_NAME] = project->name();
}

void JsonWriter::write(const BuildingBlock* bb, QJsonObject& obj) const
{
    obj[KEY_BB_NAME] = bb->name();
    obj[KEY_BB_REF] = bb->ref();
    obj[KEY_BB_ID] = m_bbHash.value(bb);
}

void JsonWriter::write(QJsonObject& jsonObj) const
{
    // Write projects
    QJsonArray projects;
    for (const Project* project : m_model.projects()) {
        QJsonObject projectObj;
        write(project, projectObj);
        projects.append(projectObj);
    }
    jsonObj[KEY_PROJECT_ARRAY] = projects;

    // Write BB
    QJsonArray buildingblocks;
    for (const BuildingBlock* bb : m_model.buildingBlocks()) {
        QJsonObject bbObj;
        write(bb, bbObj);
        projects.append(bbObj);
    }
    jsonObj[KEY_BB_ARRAY] = projects;
}

void read(DataModel& data, const QJsonObject& jsonObj)
{
    JsonReader reader(data);
    reader.read(jsonObj);
}

void write(const DataModel& model, QJsonObject& obj)
{
    JsonWriter jsonWriter(model);
    jsonWriter.write(obj);
}
