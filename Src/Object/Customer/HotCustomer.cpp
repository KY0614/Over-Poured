#include "../../Application.h"
#include "../../Manager/Generic/ResourceManager.h"
#include "../Common/AnimationController.h"
#include "HotCustomer.h"

HotCustomer::HotCustomer(void) : CustomerBase()
{
}

void HotCustomer::Draw(void)
{
	//���f������\���Ȃ�`�悵�Ȃ�
	if (!isVisible_) return;
	MV1DrawModel(transform_.modelId);
}

void HotCustomer::SetParam(void)
{
	//���f���̊�{�ݒ�
	transform_.SetModel(ResourceManager::GetInstance().LoadModelDuplicate(
		ResourceManager::SRC::HOT_CUSTOMER));
	//���f���̏��X�V
	transform_.Update();

	//���q�̃^�C�v��ݒ�
	SetType(TYPE::HOT);

	//�\�����ɐݒ�
	isVisible_ = true;
}

void HotCustomer::InitAnimation(void)
{
	//�A�j���[�V�����̏�����
	std::string path = Application::PATH_MODEL + "Player/";
	animationController_ = std::make_unique<AnimationController>(transform_.modelId);
	animationController_->Add((int)STATE::IDLE, path + "Idle.mv1", IDLE_ANIM_SPEED);
	animationController_->Add((int)STATE::WALK, path + "Walk.mv1", WALK_ANIM_SPEED);
	//�A�j���[�V�����̏�����Ԃ�ݒ�
	animationController_->Play((int)STATE::IDLE);
}
