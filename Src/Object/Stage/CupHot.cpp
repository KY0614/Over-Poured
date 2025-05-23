#include "../Manager/Generic/InputManager.h"
#include "../Object/Player.h"
#include "StageManager.h"
#include "CupHot.h"

CupHot::CupHot(const std::string objId, const float width,
	const float height, const float depth) : StageObject(objId,width,height,depth)
{
}

void CupHot::Interact(Player& player)
{
    // ホット用カップ固有のインタラクションロジック
    if (InputManager::GetInstance().IsTrgDown(KEY_INPUT_SPACE) && !player.GetIsHolding())
    {
        player.SetIsHoldiong(true);
        player.SetHoldItem(GetObjectId());
        ChangeState(STATE::HOLD);
    }
    else if (InputManager::GetInstance().IsTrgDown(KEY_INPUT_SPACE) && player.GetIsHolding() && player.GetHoldItem() == GetObjectId())
    {
        player.SetIsHoldiong(false);
        player.SetHoldItem("");
        ChangeState(STATE::PLACED);
        SetPos(StageManager::CUPHOT_POS);
    }
}

void CupHot::Init(void)
{
    StageObject::Init();
}

void CupHot::Update(void)
{
    StageObject::Update();
}

void CupHot::Draw(void)
{
    StageObject::Draw();
}
