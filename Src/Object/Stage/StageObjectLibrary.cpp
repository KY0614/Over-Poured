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
    std::ifstream ifs(Application::PATH_MODEL + "Stage/Param/ModelData.json");
    if (!ifs)return{};

    // ファイルストリームからjsonオブジェクトに変換
    json data = json::parse(ifs);
    if (!data.contains(objName))return{};

    //jsonオブジェクトから、objNameオブジェクトを取得
    const auto& value = data[objName];
    ObjectParams obj;
    
	//各パラメータを取得し、objに格納
	//存在しない場合はデフォルト値（右側の値)を設定
	obj.id_ = value.value("id", "");        
    obj.placeable_ = value.value("placeable", false);
    obj.carryable_ = value.value("carryable", false);
    obj.interactable_ = value.value("interactable", false);
    obj.interactTime_ = value.value("interactTime", 0.0f);
	obj.collisionRadius_ = value.value("collisionRadius", 0.0f);

	//acceptedItemsは配列なので、containsで存在を確認してから取得
    if (value.contains("acceptedItems"))
        obj.acceptedItems_ = value["acceptedItems"].get<std::vector<std::string>>();

    ifs.close();

    return { objName,obj };
}
