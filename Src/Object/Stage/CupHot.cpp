#include "../Manager/Generic/InputManager.h"
#include "../Object/Common/Sphere.h"
#include "../Object/Player.h"
#include "StageManager.h"
#include "CupHot.h"

CupHot::CupHot(const std::string objId, const float width,
	const float height, const float depth ,Player& player,
    std::vector<std::unique_ptr<StageObject>>& tables) : 
    StageObject(objId,width,height,depth,player),tables_(tables)
{
}

void CupHot::ItemCarry(void)
{
    auto& ins = InputManager::GetInstance();
    // ホット用カップ固有のインタラクションロジック
    if (ins.IsTrgDown(KEY_INPUT_SPACE) && GetItemState() == ITEM_STATE::PLACED)
    {
       // player_.SetIsHoldiong(true);
        player_.SetHoldItem(GetObjectId());
        ChangeItemState(ITEM_STATE::HOLD);
        isActioned_ = true;
    }
}

void CupHot::ItemPlaced(VECTOR pos)
{
    auto& ins = InputManager::GetInstance();
    if (ins.IsTrgDown(KEY_INPUT_SPACE) && GetItemState() == ITEM_STATE::HOLD)
    {
        //player_.SetIsHoldiong(false);
        player_.SetHoldItem("");
        ChangeItemState(ITEM_STATE::PLACED);
        SetPos(pos);
        isActioned_ = true;
    }
}

void CupHot::Interact(void)
{
}

void CupHot::UpdatePlaced(void)
{
    transform_.Update();
}

void CupHot::UpdateHold(void)
{
    //auto& pSphere = ;

    //SetFollowPos(pSphere.GetPos());

    transform_.pos = player_.GetSphere().GetPos();
    sphereTran_.pos = followPos_;

    transform_.Update();
}
