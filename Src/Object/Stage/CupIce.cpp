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

void CupIce::Interact(void)
{
    auto& ins = InputManager::GetInstance();
    // ホット用カップ固有のインタラクションロジック
    if (ins.IsTrgDown(KEY_INPUT_SPACE) && !player_.GetIsHolding())
    {
        player_.SetIsHoldiong(true);
        player_.SetHoldItem(GetObjectId());
        ChangeItemState(ITEM_STATE::HOLD);
    }
    else if (ins.IsTrgDown(KEY_INPUT_SPACE) && player_.GetIsHolding() && player_.GetHoldItem() == GetObjectId())
    {
        player_.SetIsHoldiong(false);
        player_.SetHoldItem("");
        ChangeItemState(ITEM_STATE::PLACED);
        SetPos(StageManager::CUPHOT_POS);
    }
}
