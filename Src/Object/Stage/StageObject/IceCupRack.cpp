#include "../Manager/Generic/InputManager.h"
#include "../Object/Player.h"
#include "IceCup.h"
#include "IceCupRack.h"

namespace {
	const std::string ICE_CUP = "Ice_Cup";	//アイス用カップ
}

IceCupRack::IceCupRack(const std::string objId, const float width,
	const float height, const float depth, Player& player):
	StageObject(objId, width, height, depth, player)
{
}

void IceCupRack::PickUp(std::vector<std::unique_ptr<StageObject>>& object)
{
	auto& ins = InputManager::GetInstance();
	if (ins.IsTrgDown(KEY_INPUT_SPACE))
	{
		object.emplace_back(std::make_unique<IceCup>(ICE_CUP, 40.0f, 30.0f, 40.0f, player_));
		object.back()->Init();
		player_.SetHoldItem(object.back()->GetObjectId());
		object.back()->ChangeItemState(ITEM_STATE::HOLD);
		isActioned_ = true;
		object.back()->SetPos(player_.GetTransform().pos);
	}
}
