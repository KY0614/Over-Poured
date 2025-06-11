#include "../../../Utility/AsoUtility.h"
#include "../Manager/Generic/InputManager.h"
#include "../Object/Stage/StageManager.h"
#include "Counter.h"

Counter::Counter(const std::string objId, const float width,
	const float height, const float depth, Player& player,
	std::vector<std::unique_ptr<StageObject>>& objects) :
	StageObject(objId, width, height, depth, player), objects_(objects)
{
}

void Counter::Update(void)
{
	StageObject::Update();
	//当たり判定用の球体をテーブルの高さと合わせる
	sphereTran_.pos.y = height_;

	for (const auto& obj : objects_)
	{
		if (AsoUtility::IsHitSpheres(obj->GetSpherePos(), obj->GetSphereRad(),
			sphereTran_.pos, rad_) && obj->GetItemState() == StageObject::ITEM_STATE::PLACED)
		{
			param_.placeable_ = false;
			break;
		}
		else if (obj->GetItemState() != StageObject::ITEM_STATE::PLACED)
		{
			param_.placeable_ = true;
		}
	}
}

void Counter::Interact(const std::string& objId)
{
}
