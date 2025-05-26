#include "../Manager/Generic/InputManager.h"
#include "../Object/Player.h"
#include "StageManager.h"
#include "Machine.h"

Machine::Machine(const std::string objId, const float width,
	const float height, const float depth,Player& player,
	StageObject& cupH, StageObject& cupI) : 
	StageObject(objId, width, height, depth,player),
	cupH_(cupH),cupI_(cupI)
{
}

void Machine::Interact(void)
{
	auto& ins = InputManager::GetInstance();
	//スペースキー押下でマシンの場所にカップを置く(とりあえず)
	if (ins.IsTrgDown(KEY_INPUT_SPACE) &&
		player_.GetIsHolding() &&
		player_.GetHoldItem() == "Cup_Hot")
	{
		player_.SetIsHoldiong(false);
		cupH_.ChangeItemState(StageObject::ITEM_STATE::PLACED);
		cupH_.SetPos(StageManager::MACHINE_POS);
	}

	////スペースキー押下でマシンの場所にカップを置く(とりあえず)
	//if (ins.IsTrgDown(KEY_INPUT_SPACE) &&
	//	!player.GetIsHolding() &&
	//	cupH_.GetState() == StageObject::STATE::PLACED)
	//{
	//	player.SetIsHoldiong(true);
	//	cupH_.ChangeState(StageObject::STATE::HOLD);
	//	player.SetHoldItem(cupH_.GetObjectId());
	//}
}

void Machine::Init(void)
{
	StageObject::Init();
}

void Machine::Update(void)
{
	StageObject::Update();
}

void Machine::Draw(void)
{
	StageObject::Draw();
}
