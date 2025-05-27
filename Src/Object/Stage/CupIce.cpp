#include "../Manager/Generic/InputManager.h"
#include "../Object/Common/Sphere.h"
#include "../Object/Player.h"
#include "StageManager.h"
#include "CupIce.h"

CupIce::CupIce(const std::string objId, const float width,
	const float height, const float depth, Player& player) : 
	StageObject(objId, width, height, depth,player)
{
}

void CupIce::ItemCarry(void)
{
    //auto& ins = InputManager::GetInstance();
    //// ホット用カップ固有のインタラクションロジック
    //if (ins.IsTrgDown(KEY_INPUT_SPACE) && !player_.GetIsHolding())
    //{
    //    player_.SetIsHoldiong(true);
    //    player_.SetHoldItem(GetObjectId());
    //    ChangeItemState(ITEM_STATE::HOLD);
    //}
    //else if (ins.IsTrgDown(KEY_INPUT_SPACE) && player_.GetIsHolding() && player_.GetHoldItem() == GetObjectId())
    //{
    //    player_.SetIsHoldiong(false);
    //    player_.SetHoldItem("");
    //    ChangeItemState(ITEM_STATE::PLACED);
    //    SetPos(StageManager::CUPICE_POS);
    //}
}

void CupIce::Interact(void)
{
}

void CupIce::UpdatePlaced(void)
{
}

void CupIce::UpdateHold(void)
{
    auto& pSphere = player_.GetSphere();
    //プレイヤー
    if (player_.GetIsHolding())
    {
        SetFollowPos(pSphere.GetPos());
    }

    transform_.pos = followPos_;
    transform_.Update();
}
