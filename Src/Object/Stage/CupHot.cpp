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
    if (ins.IsTrgDown(KEY_INPUT_SPACE))
    {
        player_.SetIsHoldiong(true);
        player_.SetHoldItem(GetObjectId());
        ChangeItemState(ITEM_STATE::HOLD);
    }
}

void CupHot::ItemPlaced(void)
{
    auto& ins = InputManager::GetInstance();
    if (ins.IsTrgDown(KEY_INPUT_SPACE))
    {
        player_.SetIsHoldiong(false);
        player_.SetHoldItem("");
        ChangeItemState(ITEM_STATE::PLACED);
        SetPos(StageManager::CUPHOT_POS);
    }

}

void CupHot::Interact(void)
{
}

void CupHot::UpdatePlaced(void)
{
}

void CupHot::UpdateHold(void)
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
