#include "../Manager/Generic/InputManager.h"
#include "../Object/Common/Sphere.h"
#include "../Object/Player.h"
#include "IceCup.h"
#include "ItemObject.h"

IceCup::IceCup(const std::string objId, const float width,
	const float height, const float depth, Player& player) : 
	StageObject(objId, width, height, depth,player)
{
	isIced_ = false;
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

void IceCup::Draw(void)
{
    int line = 3;	//行
    int lineHeight = 30;	//行

    VECTOR screenPos = ConvWorldPosToScreenPos(GetTransform().pos);
    // 変換成功
    DrawFormatString(static_cast<int>(screenPos.x),
        static_cast<int>(screenPos.y) - 100, GetColor(255, 255, 255),
        L"氷 %d", isIced_);


    StageObject::Draw();
}

void IceCup::PouredIce(void)
{
	isIced_ = true;
	objId_ = "Cup_With_Ice"; //氷が入ったカップのオブジェクトIDに変更
    object_ = StageObjectLibrary::LoadData(objId_);
    param_ = object_.second;
	return;
}

void IceCup::UpdatePlaced(void)
{
}

void IceCup::UpdateHold(void)
{
    transform_.pos = player_.GetSphere().GetPos();
}
