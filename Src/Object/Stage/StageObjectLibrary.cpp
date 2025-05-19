#include <fstream>
#include"../Libs/nlohmann/json.hpp"
#include"../Application.h"
#include "StageObjectLibrary.h"

// 長いのでnamespaceの省略
using json = nlohmann::json;

std::pair<std::string, StageObjectLibrary::ObjectParams> 
StageObjectLibrary::LoadData(const std::string& objName)
{
    std::pair<std::string, ObjectParams> objects;
    std::ifstream ifs(Application::PATH_MODEL + "Stage/ModelData.json");
    if (!ifs)return{};

    // ファイルストリームからjsonオブジェクトに変換
    json data = json::parse(ifs);
    if (!data.contains(objName))return{};

    //jsonオブジェクトから、objNameオブジェクトを取得
    const auto& value = data[objName];
    ObjectParams obj;

    obj.id_ = value.value("id", "");
    obj.name_ = value.value("name", "");
    obj.modelFile_ = value.value("model", "");
    obj.placeable_ = value.value("placeable", false);
    obj.carryable_ = value.value("carryable", false);
    obj.interactable_ = value.value("interactable", false);
    obj.interactType_ = value.value("interactType", "");
    obj.interactTime = value.value("interactTime", 0.0f);
    obj.category_ = value.value("type", "");

    if (value.contains("acceptedItems"))
        obj.acceptedItems_ = value["acceptedItems"].get<std::vector<std::string>>();

    if (value.contains("produces"))
        obj.produces_ = value["produces"].get<std::vector<std::string>>();

    ifs.close();

    return { objName,obj };

    //for (auto& [key, value] : data.items()) {
    //    ObjectParams obj;
    //    obj.id_ = value.value("id", "");
    //    obj.name_ = value.value("name", "");
    //    obj.modelFile_ = value.value("model", "");
    //    obj.placeable_ = value.value("placeable", false);
    //    obj.carryable_ = value.value("carryable", false);
    //    obj.interactable_ = value.value("interactable", false);
    //    obj.interactType_ = value.value("interactType", "");
    //    obj.interactTime = value.value("interactTime", 0.0f);
    //    obj.category_ = value.value("type", "");

    //    // acceptedItems 配列の読み込み
    //    if (value.contains("acceptedItems") && value["acceptedItems"].is_array()) {
    //        obj.acceptedItems_ = value["acceptedItems"].get<std::vector<std::string>>();
    //    }

    //    // produces 配列の読み込み（任意）
    //    if (value.contains("produces") && value["produces"].is_array()) {
    //        obj.produces_ = value["produces"].get<std::vector<std::string>>();
    //    }
    //    objects[obj.id_] = obj;
    //}

    //ifs.close();

    //return objects;
}

const StageObjectLibrary::ObjectParams* StageObjectLibrary::Get(const std::string& id)
{
    //auto it = objectMap_.find(id);
    //return (it != objectMap_.end()) ? &it->second : nullptr;
    return nullptr;
}
