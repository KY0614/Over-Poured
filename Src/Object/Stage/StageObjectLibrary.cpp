#include <fstream>
#include"../Libs/nlohmann/json.hpp"
#include"../Application.h"
#include "StageObjectLibrary.h"

// 長いのでnamespaceの省略
using json = nlohmann::json;

namespace
{
    const std::string ID_KEY = "id";
	const std::string PLACEABLE_KEY = "placeable";
	const std::string CARRYABLE_KEY = "carryable";
	const std::string INTERACTABLE_KEY = "interactable";
	const std::string INTERACT_TIME_KEY = "interactTime";
	const std::string COLLISION_RADIUS_KEY = "collisionRadius";
	const std::string ACCEPTED_ITEMS_KEY = "acceptedItems";
}

std::pair<std::string, StageObjectLibrary::ObjectParams> 
StageObjectLibrary::LoadData(const std::string& objName)
{
	//ファイルパスの指定
	const std::string filePath = "Stage/Param/ModelData.json";
    std::ifstream ifs(Application::PATH_MODEL + filePath);
    if (!ifs)return{};

    // ファイルストリームからjsonオブジェクトに変換
    json data = json::parse(ifs);
    if (!data.contains(objName))return{};

    //jsonオブジェクトから、objNameオブジェクトを取得
    const auto& value = data[objName];
    ObjectParams obj;
    
	//各パラメータを取得し、objに格納
	//存在しない場合はデフォルト値（右側の値)を設定
	obj.id_ = value.value(ID_KEY, "");
    obj.placeable_ = value.value(PLACEABLE_KEY, false);
    obj.carryable_ = value.value(CARRYABLE_KEY, false);
    obj.interactable_ = value.value(INTERACTABLE_KEY, false);
    obj.interactTime_ = value.value(INTERACT_TIME_KEY, 0.0f);
	obj.collisionRadius_ = value.value(COLLISION_RADIUS_KEY, 0.0f);

	//acceptedItemsは配列なので、containsで存在を確認してから取得
    if (value.contains(ACCEPTED_ITEMS_KEY))
        obj.acceptedItems_ = value[ACCEPTED_ITEMS_KEY].get<std::vector<std::string>>();

    ifs.close();

    return { objName,obj };
}
