#include <fstream>
#include"../Libs/nlohmann/json.hpp"
#include"../Application.h"
#include "StageObjectLibrary.h"

// �����̂�namespace�̏ȗ�
using json = nlohmann::json;

std::pair<std::string, StageObjectLibrary::ObjectParams> 
StageObjectLibrary::LoadData(const std::string& objName)
{
    std::pair<std::string, ObjectParams> objects;
    std::ifstream ifs(Application::PATH_MODEL + "Stage/ModelData.json");
    if (!ifs)return{};

    // �t�@�C���X�g���[������json�I�u�W�F�N�g�ɕϊ�
    json data = json::parse(ifs);
    if (!data.contains(objName))return{};

    //json�I�u�W�F�N�g����AobjName�I�u�W�F�N�g���擾
    const auto& value = data[objName];
    ObjectParams obj;

    obj.id_ = value.value("id", "");
    obj.name_ = value.value("name", "");
    obj.placeable_ = value.value("placeable", false);
    obj.carryable_ = value.value("carryable", false);
    obj.interactable_ = value.value("interactable", false);
    obj.interactType_ = value.value("type", "");
    obj.interactTime_ = value.value("interactTime", 0.0f);
    obj.category_ = value.value("type", "");

    if (value.contains("acceptedItems"))
        obj.acceptedItems_ = value["acceptedItems"].get<std::vector<std::string>>();

    if (value.contains("produces"))
        obj.produces_ = value["produces"].get<std::vector<std::string>>();

    ifs.close();

    return { objName,obj };
}
