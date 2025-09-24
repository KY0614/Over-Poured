#include "../Manager/Generic/InputManager.h"
#include "../Manager/GameSystem/SoundManager.h"
#include "../Object/Common/Sphere.h"
#include "../Object/Player.h"
#include "ItemObject.h"

ItemObject::ItemObject(const std::string objId,Player& player) :
	StageObject(objId, player)
{
	isIced_ = false;   
}

void ItemObject::ItemCarry(void)
{
    auto& ins = InputManager::GetInstance();
    auto& sound = SoundManager::GetInstance();
    //アイテムオブジェクトのインタラクト処理（持ち運ぶ）
    if (ins.IsInputTriggered("Interact") && GetItemState() == ITEM_STATE::PLACED)
    {
		//SE再生
        sound.Play(SoundManager::SOUND::PICK_UP);
		//プレイヤーの持ち物にアイテムIDをセット
        player_.SetHoldItem(param_.id_);
		//アイテムの状態をHOLDに変更
        ChangeItemState(ITEM_STATE::HOLD);
		isActioned_ = true; //アクションが実行されたフラグを立てる
    }
}

void ItemObject::ItemPlaced(VECTOR pos)
{
    auto& ins = InputManager::GetInstance();
    auto& sound = SoundManager::GetInstance();
    //アイテムオブジェクトのインタラクト処理（設置）
    if (ins.IsInputTriggered("Interact") && GetItemState() == ITEM_STATE::HOLD)
    {
		//SE再生
        sound.Play(SoundManager::SOUND::PUT_ON);
		//プレイヤーの持ち物を空にする
        player_.SetHoldItem("");
		//アイテムの状態を設置済みに変更
        ChangeItemState(ITEM_STATE::PLACED);
		//アイテムの座標を設置位置に変更
        SetPos(pos);
		isActioned_ = true; //アクションが実行されたフラグを立てる
    }
}

void ItemObject::PouredIce(void)
{
	//カップに氷を注ぐ処理
    isIced_ = true;
    //氷が入ったカップのオブジェクトIDに変更
    objId_ = CUP_WITH_ICE;
	//データの再読み込み
    param_ = StageObjectLibrary::LoadData(objId_).second;
}

void ItemObject::Draw(void)
{
    StageObject::Draw();
}

void ItemObject::UpdatePlaced(void)
{
}

void ItemObject::UpdateHold(void)
{
    //HOLD状態のアイテムはプレイヤーの座標を追従する
    transform_.pos = player_.GetSphere().GetPos();
}
