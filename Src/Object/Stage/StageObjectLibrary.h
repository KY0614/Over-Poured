#pragma once
#include <string>
#include <vector>
#include <unordered_map>


class StageObjectLibrary
{
public:

    /// <summary>
    /// オブジェクトの属性を保持する構造体
    /// </summary>
    struct ObjectParams 
    {
        std::string id_ = "";               //識別子
        bool placeable_ = false;            //上に物を置けるか（例：机）
        bool carryable_ = false;            //持ち運びできるか（例：カップ）
        bool interactable_ = false;         //プレイヤーがアクション可能か

        float interactTime_ = 0.0f;         //インタラクトにかかる時間（例：注ぐ時間）

        std::vector<std::string> acceptedItems_;   //インタラクト対象にできるアイテム（例：コップだけなど）

		float collisionRadius_ = 0.0f;    //当たり判定の半径
    };

    /// <summary>
    /// 指定されたオブジェクト名に基づいてデータを読み込む
    /// </summary>
    /// <param name="objName">データを読み込む対象のオブジェクト名</param>
    /// <returns>オブジェクト名と対応するパラメータ</returns>
    static std::pair<std::string, ObjectParams> LoadData(const std::string& objName);
};

