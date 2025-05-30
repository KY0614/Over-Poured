#include "../Manager/Generic/InputManager.h"
#include "../Object/Common/Sphere.h"
#include "../Object/Player.h"
#include "../Object/Stage/StageManager.h"
#include "IceCup.h"

IceCup::IceCup(const std::string objId, const float width,
	const float height, const float depth, Player& player) : 
	StageObject(objId, width, height, depth,player)
{
}

void IceCup::ItemCarry(void)
{
    auto& ins = InputManager::GetInstance();
    // ホット用カップ固有のインタラクションロジック
    if (ins.IsTrgDown(KEY_INPUT_SPACE) && GetItemState() == ITEM_STATE::PLACED)
    {
        player_.SetHoldItem(GetObjectId());
        ChangeItemState(ITEM_STATE::HOLD);
        isActioned_ = true;
    }
}

void IceCup::ItemPlaced(VECTOR pos)
{
    auto& ins = InputManager::GetInstance();
    if (ins.IsTrgDown(KEY_INPUT_SPACE) && GetItemState() == ITEM_STATE::HOLD)
    {
        player_.SetHoldItem("");
        ChangeItemState(ITEM_STATE::PLACED);
        SetPos(pos);
        isActioned_ = true;
    }
}

void IceCup::UpdatePlaced(void)
{
}

void IceCup::UpdateHold(void)
{
    transform_.pos = player_.GetSphere().GetPos();
}
