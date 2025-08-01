#include<DxLib.h>
#include "../../Manager/Generic/SceneManager.h"
#include "../Common/AnimationController.h"
#include "../../Utility/AsoUtility.h"
#include "CustomerBase.h"

namespace
{
	const VECTOR CUSTOMER_SCALE = { 0.7f, 0.7f, 0.7f };	//���q�̑傫��
	const VECTOR COUNTER_POS = { 221.0f, 0.0f, 271.0f };//�J�E���^�[�̈ʒu
	const float CUSTOMER_ROT_Y = -90.0f; // �ڋq�̏���Y����]�p�x
	const float MOVE_SPEED = 1.5f;
}

CustomerBase::CustomerBase(void)
{
	animationController_ = nullptr;
	type_ = TYPE::NONE;
	state_ = STATE::IDLE;
	reaction_ = REACTION::NONE;

	isVisible_ = false;

	stepRotTime_ = 0.0f;
}

void CustomerBase::Init(VECTOR pos)
{
	//���f����񏉊���
	transform_.scl = CUSTOMER_SCALE;	//�傫��
	transform_.pos = pos;				//�ʒu�������Ŏw��
	transform_.quaRot = Quaternion();	//��]�͏�����
	transform_.quaRotLocal =			//���[�J����]�ݒ�
		Quaternion::Euler({ 0.0f, AsoUtility::Deg2RadF(CUSTOMER_ROT_Y), 0.0f });
	//���f�������X�V
	transform_.Update();
	//���q�̃p�����[�^�[�ݒ�
	SetParam();
	//�A�j���[�V����������
	InitAnimation();
}

void CustomerBase::Update(void)
{
	transform_.Update();
	animationController_->Update();

	//��Ԃ��Ƃ̃A�j���[�V����
	StateAnimation();
	//Y����]
	RotateY();

	//Y����]�𔽉f
	transform_.quaRot = Quaternion::Quaternion();
	transform_.quaRot = transform_.quaRot.Mult(customerRotY_);
}

void CustomerBase::Move(void)
{
	//�E�Ɉړ�������
	transform_.pos.x += MOVE_SPEED;
}

void CustomerBase::RotateY(void)
{
	//��]�ɂ����鎞�Ԃ��o�ߎ��Ō��炷
	stepRotTime_ -= SceneManager::GetInstance().GetDeltaTime();

	//��]�̋��ʕ��
	customerRotY_ = Quaternion::Slerp(
		customerRotY_, goalQuaRot_, (TIME_ROT - stepRotTime_) / TIME_ROT);
}

bool CustomerBase::CollisionCounter(void)
{
	if (AsoUtility::IsHitSpherePoint(COUNTER_POS, 30, GetTransform().pos) && isVisible_)
	{
		//�������Ă�����true��Ԃ�
		return true;
	}
	return false;
}

bool CustomerBase::CheckCounterToCustomer(void)
{
	//���q���J�E���^�[�̈ʒu�𒴂�������������true��Ԃ�
	if (GetTransform().pos.x > COUNTER_POS.x)
	{
		return true;
	}
	return false;
}

void CustomerBase::SetGoalRotate(double rotRad)
{ 
	Quaternion axis =
		Quaternion::AngleAxis(
			rotRad, AsoUtility::AXIS_Y);

	//���ݐݒ肳��Ă����]�Ƃ̊p�x�������
	double angleDiff = Quaternion::Angle(axis, goalQuaRot_);

	//�������l
	if (angleDiff > 0.1)
	{
		stepRotTime_ = TIME_ROT;
	}
	goalQuaRot_ = axis;
}

void CustomerBase::Rotate(void)
{
	stepRotTime_ -= SceneManager::GetInstance().GetDeltaTime();
}

void CustomerBase::StateAnimation(void)
{
	//��Ԃ��Ƃɂ��A�j���[�V�����̐؂�ւ�
	switch (state_)
	{
	case CustomerBase::STATE::IDLE:
		animationController_->Play((int)STATE::IDLE);
		break;

	case CustomerBase::STATE::WALK:
		animationController_->Play((int)STATE::WALK);
		break;
	default:
		break;
	}
}
