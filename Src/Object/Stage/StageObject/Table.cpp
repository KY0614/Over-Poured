#include "../Utility/CommonUtility.h"
#include "../Object/Common/Sphere.h"
#include "../Object/Player.h"
#include "Table.h"

namespace
{
	const std::string HOT_COFFEE = "HotCoffee";	//ホットコーヒーのオブジェクトID
	const std::string ICE_COFFEE = "IceCoffee";	//アイスコーヒーのオブジェクトID
}

Table::Table(const std::string& objId,
	Player& player,
	std::vector<std::unique_ptr<StageObject>>& objects) :
	StageObject(objId, player),
	objects_(objects)
{
}

void Table::Update(void)
{
	StageObject::Update();

	//テーブルの上にオブジェクトが置かれていないかどうか
	for (const auto& obj : objects_)
	{
		//当たっているかどうかの判定情報
		bool isHit = CommonUtility::IsHitSpheres(
			obj->GetSphere().GetPos(), obj->GetSphere().GetRadius(),
			GetSphere().GetPos(), GetSphere().GetRadius());

		if (isHit &&
			(obj->GetParam().id_ == COFFEE_MACHINE || obj->GetParam().id_ == ICE_DISPENSER ||
			obj->GetParam().id_ == HOT_CUP_RACK || obj->GetParam().id_ == ICE_CUP_RACK ||
			obj->GetParam().id_ == CUP_LID_RACK || obj->GetParam().id_ == BERRY_SWEETS_RACK ||
			obj->GetParam().id_ == CHOCO_SWEETS_RACK))
		{
			//置かれている場合は、置けない状態にする
			param_.placeable_ = false;
			break;
		}
		if (isHit && obj->GetItemState() == StageObject::ITEM_STATE::PLACED)
		{
			//置かれている場合は、置けない状態にする
			param_.placeable_ = false;
			break;
		}
		else if(obj->GetItemState() != StageObject::ITEM_STATE::PLACED)
		{
			param_.placeable_ = true;
		}
	}
}