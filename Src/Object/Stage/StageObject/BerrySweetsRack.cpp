#include "../Manager/Generic/InputManager.h"
#include "../../Common/Sphere.h"
#include "../Object/Player.h"
#include "Sweets.h"
#include "BerrySweetsRack.h"

BerrySweetsRack::BerrySweetsRack(const std::string objId,
	const float width, const float height,
	const float depth, Player& player) :
	StageObject(objId, width, height, depth, player)
{
}

void BerrySweetsRack::PickUp(std::string rackName, std::vector<std::unique_ptr<StageObject>>& object)
{
	auto& ins = InputManager::GetInstance();
	if (ins.IsTrgDown(KEY_INPUT_SPACE))
	{
		//スイーツを取り出す
		object.emplace_back(std::make_unique<Sweets>("Sweets_Strawberry", 40.0f, 30.0f, 40.0f, player_));
		object.back()->SetPos(player_.GetSphere().GetPos());
		object.back()->Init();
		player_.SetHoldItem(object.back()->GetObjectId());
		object.back()->ChangeItemState(ITEM_STATE::HOLD);
		isActioned_ = true;
	}
}
