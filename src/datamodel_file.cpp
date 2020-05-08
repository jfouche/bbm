#include "datamodel_file.h"
#include "json_utils.hpp"
#include <map>


static const char* KEY_PROJECT_ARRAY = "projects";
static const char* KEY_PROJECT_NAME = "name";
static const char* KEY_BB_ARRAY = "building_blocks";
static const char* KEY_BB_NAME = "name";
static const char* KEY_BB_REF = "ref";
static const char* KEY_BB_ID = "id";
static const char* KEY_BB_MATURITY = "maturity";
static const char* KEY_BB_INFO = "info";
static const char* KEY_BB_CHILDREN = "children";


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

QString MaturityToString(BuildingBlock::Maturity m)
{
    switch (m) {
    case BuildingBlock::Maturity::A : return "A";
    case BuildingBlock::Maturity::B : return "B";
    case BuildingBlock::Maturity::C : return "C";
    case BuildingBlock::Maturity::D : return "D";
    case BuildingBlock::Maturity::E : return "E";
    case BuildingBlock::Maturity::F : return "F";
    }
    throw std::invalid_argument("Bad maturity");
}

namespace json {
template<>
BuildingBlock::Maturity as(const key_value& pair)
{
    QString s=as<QString>(pair);
    if (s == "A") return BuildingBlock::Maturity::A;
    if (s == "B") return BuildingBlock::Maturity::B;
    if (s == "C") return BuildingBlock::Maturity::C;
    if (s == "D") return BuildingBlock::Maturity::D;
    if (s == "E") return BuildingBlock::Maturity::E;
    if (s == "F") return BuildingBlock::Maturity::F;
    throw invalid_type_exception(KEY_BB_MATURITY);
}
}

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
    std::map<const BuildingBlock*, int> m_bbMap;
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
    std::map<int, BuildingBlock*> m_bbHash;
    std::map<BuildingBlock*, std::vector<int>> m_bbChildren;
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
    bb->setMaturity(json::as<BuildingBlock::Maturity>(jsonObj, KEY_BB_MATURITY));
    bb->setInfo(json::as<QString>(jsonObj, KEY_BB_INFO));
    int id = json::as<int>(jsonObj, KEY_BB_ID);
    m_bbHash[id] = bb;
    for (auto value : json::as<QJsonArray>(jsonObj, KEY_BB_CHILDREN)) {
        m_bbChildren[bb].push_back(json::as<int>(value));
    }
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
    for (auto value : json::as<QJsonArray>(jsonObj, KEY_BB_ARRAY)) {
        QJsonObject bbObj = json::as<QJsonObject>(bbIndexValue(value));
        auto bb = m_model.addBuildingBlock();
        read(bb, bbObj);
    }

    // link BB children to BB
    for (const auto& kv: m_bbChildren) {
        BuildingBlock* bb = kv.first;
        for (int id : kv.second) {
            auto it = m_bbHash.find(id);
            if (it->first) {
                bb->add(it->second);
            }
        }
    }

    // link BB children to projects
    /// TODO
}

JsonWriter::JsonWriter(const DataModel& model)
    : m_model(model)
{
    int id = 1;
    for (const BuildingBlock* bb : m_model.buildingBlocks()) {
        m_bbMap[bb] = id++;
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
    obj[KEY_BB_ID] = m_bbMap.at(bb);
    obj[KEY_BB_MATURITY] = MaturityToString(bb->maturity());
    obj[KEY_BB_INFO] = bb->info();

    QJsonArray children;
    for (const BuildingBlock* childBb : bb->children()) {
        auto it = m_bbMap.find(childBb);
        if (it->first) {
            children.append( QJsonValue(it->second));
        }
    }
    obj[KEY_BB_CHILDREN] = children;
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
        buildingblocks.append(bbObj);
    }
    jsonObj[KEY_BB_ARRAY] = buildingblocks;
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
