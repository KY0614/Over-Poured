#include "../Manager/Generic/InputManager.h"
#include "../Object/Common/Sphere.h"
#include "../Object/Player.h"
#include "../Object/Stage/StageManager.h"
#include "HotCup.h"

HotCup::HotCup(const std::string objId, const float width,
	const float height, const float depth ,Player& player) : 
    StageObject(objId,width,height,depth,player)
{
}

void HotCup::ItemCarry(void)
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

void HotCup::ItemPlaced(VECTOR pos)
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

void HotCup::UpdatePlaced(void)
{
    transform_.Update();
}

void HotCup::UpdateHold(void)
{
    //auto& pSphere = ;

    //SetFollowPos(pSphere.GetPos());

    transform_.pos = player_.GetSphere().GetPos();
    sphereTran_.pos = followPos_;

    transform_.Update();
}
