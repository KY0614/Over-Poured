#include "../Manager/Generic/InputManager.h"
#include "../Object/Common/Sphere.h"
#include "../Object/Player.h"
#include "ItemObject.h"

ItemObject::ItemObject(const std::string objId,
	const float width, const float height,
	const float depth, Player& player) :
	StageObject(objId, width, height, depth, player)
{
}

void ItemObject::ItemCarry(void)
{
    auto& ins = InputManager::GetInstance();
    // ホット用カップ固有のインタラクションロジック
    if (ins.IsInputTriggered("Interact") && GetItemState() == ITEM_STATE::PLACED)
    {
        player_.SetHoldItem(param_.id_);
        ChangeItemState(ITEM_STATE::HOLD);
        isActioned_ = true;
    }
}

void ItemObject::ItemPlaced(VECTOR pos)
{
    auto& ins = InputManager::GetInstance();
    if (ins.IsInputTriggered("Interact") && GetItemState() == ITEM_STATE::HOLD)
    {
        player_.SetHoldItem("");
        ChangeItemState(ITEM_STATE::PLACED);
        SetPos(pos);
        isActioned_ = true;
    }
}

void ItemObject::PouredIce(void)
{
    isIced_ = true;
    objId_ = "Cup_With_Ice"; //氷が入ったカップのオブジェクトIDに変更
    object_ = StageObjectLibrary::LoadData(objId_);
    param_ = object_.second;
    return;
}

void ItemObject::UpdatePlaced(void)
{
}

void ItemObject::UpdateHold(void)
{
    transform_.pos = player_.GetSphere().GetPos();
}
