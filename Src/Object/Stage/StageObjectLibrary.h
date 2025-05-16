#pragma once
#include <string>
#include <vector>
#include <unordered_map>


class StageObjectLibrary
{
public:

    struct ObjectParams{
        std::string id_;               //識別子
        std::string modelFile_;        //モデルファイル名
        bool placeable_ = false;       //上に物を置けるか（例：机）
        bool carryable_ = false;       //持ち運びできるか（例：カップ）
        bool interactable_ = false;    //プレイヤーがアクション可能か

        std::string interactType_;     //"place", "machine", "serve", "charge", など動作タイプ（任意）
        float interactTime = 0.0f;    //インタラクトにかかる時間（例：注ぐ時間）

        std::vector<std::string> acceptedItems_; //インタラクト対象にできるアイテム（例：コップだけなど）

        // ゲーム的なカテゴリ（分類）例："machine", "item", "counter"
        std::string category_;

        //// 見た目で判断できる属性（ホット/アイス用）など
        //std::string visualHin_t;
    };

    static bool LoadData(const std::string& jsonFilePath);
    static const ObjectParams& Get(const std::string& id);

private:

    static std::unordered_map<std::string, ObjectParams> objectMap_;

};

