#include "../Manager/Generic/InputManager.h"
#include "../Object/Player.h"
#include "HotCup.h"
#include "HotCupRack.h"

namespace {
	const std::string NO_ITEM = "";	//ホット用カップ
	const std::string HOT_CUP = "Hot_Cup";	//ホット用カップ
	const std::string ICE_CUP = "Ice_Cup";	//アイス用カップ
	const std::string CUP_WITH_ICE = "Cup_With_Ice";		//アイス用カップ
	const std::string COFFEE_MACHINE = "Coffee_Machine";	//コーヒーマシン
}

HotCupRack::HotCupRack(const std::string objId, const float width,
	const float height, const float depth, Player& player):StageObject(objId, width, height, depth, player)
{
}

void HotCupRack::PickUp(std::vector<std::unique_ptr<StageObject>>& object)
{
	auto& ins = InputManager::GetInstance();
	if (ins.IsTrgDown(KEY_INPUT_SPACE))
	{
		object.emplace_back(std::make_unique<HotCup>(HOT_CUP, 40.0f, 30.0f, 40.0f, player_));
		object.back()->Init();
		player_.SetHoldItem(object.back()->GetObjectId());
		object.back()->ChangeItemState(ITEM_STATE::HOLD);
		isActioned_ = true;
		object.back()->SetPos(player_.GetTransform().pos);
	}
}

void HotCupRack::UpdateInActive(void)
{
}

void HotCupRack::UpdateActive(void)
{
}