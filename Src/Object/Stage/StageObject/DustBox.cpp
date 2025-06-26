#include "../../Player.h"
#include "FollowingObject.h"
#include "DustBox.h"

namespace {
	const std::string HOT_COFFEE = "Hot_Coffee";		//ホットコーヒー
	const std::string ICE_COFFEE = "Ice_Coffee";		//アイスコーヒー
}

DustBox::DustBox(const std::string objId, const float width,
	const float height, const float depth, Player& player,
	std::vector<std::unique_ptr<StageObject>>& objects) :
	StageObject(objId, width, height, depth, player), objects_(objects)
{
}

void DustBox::Interact(const std::string& objId)
{
	auto& items = param_.acceptedItems_;
	//objIdがインタラクト対象物に存在するかどうか
	bool isAccepted = std::find(items.begin(), items.end(), objId) != items.end();
	if (!isAccepted)return;	//存在しなかったら処理しない

	// プレイヤーが持っているアイテム名を取得
	std::string heldItem = player_.GetHoldItem();

	// コーヒー本体のインデックスを探す
	int coffeeIndex = -1;
	for (int i = 0; i < objects_.size(); ++i)
	{
		if ((objects_[i]->GetParam().id_ == HOT_COFFEE || objects_[i]->GetParam().id_ == ICE_COFFEE) &&
			objects_[i]->GetItemState() == StageObject::ITEM_STATE::HOLD)
		{
			coffeeIndex = i;
			break;
		}
	}

	// 蓋付きコーヒーの場合は蓋も削除
	if (coffeeIndex != -1 && objects_[coffeeIndex]->IsLidOn())
	{
		// 蓋のインデックスを探す
		for (int i = 0; i < objects_.size(); ++i)
		{
			// dynamic_castでCupLid型に変換し、親参照を比較
			//蓋を削除する
			FollowingObject* lid = dynamic_cast<FollowingObject*>(objects_[i].get());
			if (lid && &(lid->GetFollowedObj()) == objects_[coffeeIndex].get())
			{
				objects_.erase(objects_.begin() + i);
				continue; // 蓋を削除した後、次のループへ
			}
			++i;
		}
		// コーヒー本体も削除
		objects_.erase(objects_.begin() + coffeeIndex);
	}
	else
	{
		// 通常のオブジェクト削除
		for (auto it = objects_.begin(); it != objects_.end(); ++it)
		{
			if ((*it)->GetParam().id_ == heldItem &&
				(*it)->GetItemState() == StageObject::ITEM_STATE::HOLD)
			{
				objects_.erase(it);
				break;
			}
		}
	}

	// プレイヤーの持ち物状態をリセット
	player_.SetHoldItem("");
	player_.SetIsHoldiong(false);
}
