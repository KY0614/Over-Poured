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
    //�A�C�e���I�u�W�F�N�g�̃C���^���N�g�����i�����^�ԁj
    if (ins.IsInputTriggered("Interact") && GetItemState() == ITEM_STATE::PLACED)
    {
		//SE�Đ�
        sound.Play(SoundManager::SOUND::PICK_UP);
		//�v���C���[�̎������ɃA�C�e��ID���Z�b�g
        player_.SetHoldItem(param_.id_);
		//�A�C�e���̏�Ԃ�HOLD�ɕύX
        ChangeItemState(ITEM_STATE::HOLD);
		isActioned_ = true; //�A�N�V���������s���ꂽ�t���O�𗧂Ă�
    }
}

void ItemObject::ItemPlaced(VECTOR pos)
{
    auto& ins = InputManager::GetInstance();
    auto& sound = SoundManager::GetInstance();
    //�A�C�e���I�u�W�F�N�g�̃C���^���N�g�����i�ݒu�j
    if (ins.IsInputTriggered("Interact") && GetItemState() == ITEM_STATE::HOLD)
    {
		//SE�Đ�
        sound.Play(SoundManager::SOUND::PUT_ON);
		//�v���C���[�̎���������ɂ���
        player_.SetHoldItem("");
		//�A�C�e���̏�Ԃ�ݒu�ς݂ɕύX
        ChangeItemState(ITEM_STATE::PLACED);
		//�A�C�e���̍��W��ݒu�ʒu�ɕύX
        SetPos(pos);
		isActioned_ = true; //�A�N�V���������s���ꂽ�t���O�𗧂Ă�
    }
}

void ItemObject::PouredIce(void)
{
	//�J�b�v�ɕX�𒍂�����
    isIced_ = true;
    //�X���������J�b�v�̃I�u�W�F�N�gID�ɕύX
    objId_ = CUP_WITH_ICE;
	//�f�[�^�̍ēǂݍ���
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
    //HOLD��Ԃ̃A�C�e���̓v���C���[�̍��W��Ǐ]����
    transform_.pos = player_.GetSphere().GetPos();
}
