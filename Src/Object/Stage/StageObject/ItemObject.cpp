#include "../../../Common/DebugDrawFormat.h"
#include "../../../Utility/AsoUtility.h"
#include "../Manager/Generic/InputManager.h"
#include "../Manager/GameSystem/SoundManager.h"
#include "../Object/Common/Sphere.h"
#include "../Object/Player.h"
#include "ItemObject.h"

ItemObject::ItemObject(const std::string objId,
	const float height,Player& player) :
	StageObject(objId, height, player)
{
	isIced_ = false;   
}

void ItemObject::ItemCarry(void)
{
    auto& ins = InputManager::GetInstance();
    auto& sound = SoundManager::GetInstance();
    //�A�C�e���I�u�W�F�N�g�̃C���^���N�g�����i�����^�ԁj
    if (ins.IsInputTriggered("Interact") && GetItemState() == ITEM_STATE::PLACED)
    {
        sound.Play(SoundManager::SOUND::PICK_UP);
        player_.SetHoldItem(param_.id_);
        ChangeItemState(ITEM_STATE::HOLD);
        isActioned_ = true;
    }
}

void ItemObject::ItemPlaced(VECTOR pos)
{
    auto& ins = InputManager::GetInstance();
    auto& sound = SoundManager::GetInstance();
    //�A�C�e���I�u�W�F�N�g�̃C���^���N�g�����i�ݒu�j
    if (ins.IsInputTriggered("Interact") && GetItemState() == ITEM_STATE::HOLD)
    {
        sound.Play(SoundManager::SOUND::PUT_ON);
        player_.SetHoldItem("");
        ChangeItemState(ITEM_STATE::PLACED);
        SetPos(pos);
        isActioned_ = true;
    }
}

void ItemObject::PouredIce(void)
{
    isIced_ = true;
    objId_ = CUP_WITH_ICE; //�X���������J�b�v�̃I�u�W�F�N�gID�ɕύX
    param_ = StageObjectLibrary::LoadData(objId_).second;
    return;
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
    //HOLD��Ԃ̃A�C�e���̓v���C���[�̍��W��Ǐ]����
    transform_.pos = player_.GetSphere().GetPos();
}
