#include <fstream>
#include"../Libs/nlohmann/json.hpp"
#include "StageObjectLibrary.h"

bool StageObjectLibrary::LoadData(const std::string& jsonFilePath)
{
    std::ifstream ifs(jsonFilePath);
    if (!ifs.is_open()) {
        return false;
    }

    nlohmann::json j;
    try {
        ifs >> j;
    }
    catch (const nlohmann::json::parse_error& e) {
        // パースエラー
        return false;
    }

    if (!j.is_object()) {
        return false;
    }

    for (auto& [id, obj] : j.items()) {
        ObjectParams params;
        params.id_ = id;

        if (obj.contains("modelFile") && obj["modelFile"].is_string())
            params.modelFile_ = obj["modelFile"].get<std::string>();

        if (obj.contains("placeable") && obj["placeable"].is_boolean())
            params.placeable_ = obj["placeable"].get<bool>();

        if (obj.contains("carryable") && obj["carryable"].is_boolean())
            params.carryable_ = obj["carryable"].get<bool>();

        if (obj.contains("interactable") && obj["interactable"].is_boolean())
            params.interactable_ = obj["interactable"].get<bool>();

        if (obj.contains("interactType") && obj["interactType"].is_string())
            params.interactType_ = obj["interactType"].get<std::string>();

        if (obj.contains("interactTime") && obj["interactTime"].is_number())
            params.interactTime = obj["interactTime"].get<float>();

        if (obj.contains("acceptedItems") && obj["acceptedItems"].is_array()) {
            for (auto& item : obj["acceptedItems"]) {
                if (item.is_string())
                    params.acceptedItems_.push_back(item.get<std::string>());
            }
        }

        if (obj.contains("category") && obj["category"].is_string())
            params.category_ = obj["category"].get<std::string>();

        objectMap_.emplace(id, std::move(params));
    }

    return true;
}

const StageObjectLibrary::ObjectParams& StageObjectLibrary::Get(const std::string& id)
{
	return ObjectParams();
}
