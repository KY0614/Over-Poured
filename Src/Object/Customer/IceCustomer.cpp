#include "../../Application.h"
#include "../../Utility/AsoUtility.h"
#include "../../Manager/Generic/SceneManager.h"
#include "../../Manager/Generic/ResourceManager.h"
#include "../Common/AnimationController.h"
#include "IceCustomer.h"

IceCustomer::IceCustomer(void): CustomerBase()
{
}

void IceCustomer::Draw(void)
{
	//���f������\���Ȃ�`�悵�Ȃ�
	if (!isVisible_) return;
	MV1DrawModel(transform_.modelId);
}

void IceCustomer::SetParam(void)
{
	//���f���̊�{�ݒ�
	transform_.SetModel(ResourceManager::GetInstance().LoadModelDuplicate(
		ResourceManager::SRC::ICE_CUSTOMER));
	//���f���̏��X�V
	transform_.Update();

	//���q�̃^�C�v��ݒ�
	SetType(TYPE::ICE);

	//�\�����ɐݒ�
	isVisible_ = true;
}

void IceCustomer::InitAnimation(void)
{
	//�A�j���[�V�����̏�����
	std::string path = Application::PATH_MODEL + "Player/";
	animationController_ = std::make_unique<AnimationController>(transform_.modelId);
	animationController_->Add((int)STATE::IDLE, path + "Idle.mv1", IDLE_ANIM_SPEED);
	animationController_->Add((int)STATE::WALK, path + "Walk.mv1", WALK_ANIM_SPEED);

	animationController_->Play((int)STATE::IDLE);
}