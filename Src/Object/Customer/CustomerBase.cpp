#include<DxLib.h>
#include<EffekseerForDXLib.h>
#include "../../Manager/Generic/SceneManager.h"
#include "../../Manager/Generic/ResourceManager.h"
#include "../Common/AnimationController.h"
#include "../../Utility/AsoUtility.h"
#include "CustomerBase.h"

namespace//����cpp���ł����g��Ȃ��萔
{
	const VECTOR CUSTOMER_SCALE = { 0.7f, 0.7f, 0.7f };	//���q�̑傫��
	const VECTOR COUNTER_POS = { 221.0f, 0.0f, 271.0f };//�J�E���^�[�̈ʒu
	const float CUSTOMER_ROT_Y = -90.0f;	//���q�̏���Y����]�p�x
	const float MOVE_SPEED = 1.5f;			//���q�̈ړ����x
	const float COUNTER_RADIUS = 30.0f;		//�J�E���^�[�̋��̓����蔻��̔��a
	const float ROT_TIME = 1.0f;			//��]�����܂ł̎���
}

CustomerBase::CustomerBase(void)
{
	animationController_ = nullptr;
	type_ = TYPE::NONE;
	state_ = STATE::IDLE;
	reaction_ = REACTION::NONE;

	isVisible_ = false;

	stepRotTime_ = -1.0f;
}

void CustomerBase::Init(const VECTOR pos)
{
	//���q�̃p�����[�^�[�ݒ�
	SetParam();
	//�A�j���[�V����������
	InitAnimation();
	//���f����񏉊���
	transform_.scl = CUSTOMER_SCALE;	//�傫��
	transform_.pos = pos;				//�ʒu�������Ŏw��
	transform_.quaRot = Quaternion();	//��]�͏�����
	transform_.quaRotLocal =			//���[�J����]�ݒ�
		Quaternion::Euler({ 0.0f, AsoUtility::Deg2RadF(CUSTOMER_ROT_Y), 0.0f });
	//���f�������X�V
	transform_.Update();

	effektHappyResId_ = ResourceManager::GetInstance().Load(
		ResourceManager::SRC::HAPPY_STAR).handleId_;

	chestFrmNo_ = MV1SearchFrame(transform_.modelId, L"mixamorig:Head");
}

void CustomerBase::Update(void)
{
	transform_.Update();
	animationController_->Update();

	//��Ԃ��Ƃ̃A�j���[�V����
	StateAnimation();
	StateReaction();
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

void CustomerBase::SetGoalRotate(double rotRad)
{ 
	//��]����Y���ɐݒ肵�ĉ�]�p�x��ݒ�
	Quaternion axis =
		Quaternion::AngleAxis(
			rotRad, AsoUtility::AXIS_Y);

	//���ݐݒ肳��Ă����]�Ƃ̊p�x�������
	double angleDiff = Quaternion::Angle(axis, goalQuaRot_);

	//�p�x����0.1�ȏ�Ȃ��]���Ԃ�ݒ�
	if (angleDiff > 0.1)
	{
		stepRotTime_ = ROT_TIME;
	}
	//�ڕW�̉�]��ݒ�
	goalQuaRot_ = axis;
}

bool CustomerBase::CollisionCounter(void)
{
	//�\�����̂��q���J�E���^�[�̋��̓����蔻��ɓ������Ă��邩
	if (AsoUtility::IsHitSpherePoint(COUNTER_POS, COUNTER_RADIUS,
		GetTransform().pos) && isVisible_)
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

void CustomerBase::RotateY(void)
{
	//��]�ɂ����鎞�Ԃ��o�ߎ��Ō��炷
	stepRotTime_ -= SceneManager::GetInstance().GetDeltaTime();

	//��]�̋��ʕ��
	customerRotY_ = Quaternion::Slerp(
		customerRotY_, goalQuaRot_, (ROT_TIME - stepRotTime_) / ROT_TIME);
}

void CustomerBase::StateAnimation(void)
{
	//��Ԃ��Ƃɂ��A�j���[�V�����̐؂�ւ�
	switch (state_)
	{
	case CustomerBase::STATE::IDLE:	//�ҋ@���
		animationController_->Play((int)STATE::IDLE);
		break;

	case CustomerBase::STATE::WALK:	//���s���
		animationController_->Play((int)STATE::WALK);
		break;
	default:
		break;
	}
}

void CustomerBase::StateReaction(void)
{
	//��Ԃ��Ƃɂ��A�j���[�V�����̐؂�ւ�
	switch (reaction_)
	{
	case CustomerBase::REACTION::NONE:
		
		break;

	case CustomerBase::REACTION::BAD:
		
		break;

	case CustomerBase::REACTION::SOSO:
		
		break;
	case CustomerBase::REACTION::GOOD:
		EffektHappyStar();
		break;

	default:
		break;
	}
}

void CustomerBase::EffektHappyStar(void)
{
	effektHappyPlayId_ = PlayEffekseer3DEffect(effektHappyResId_);

	SetSpeedPlayingEffekseer3DEffect(effektHappyPlayId_, 2.5f);

	float scale = 30.0f;
	SetScalePlayingEffekseer3DEffect(
		effektHappyPlayId_,
		scale,
		scale,
		scale
	);

	VECTOR pos = MV1GetFramePosition(transform_.modelId, chestFrmNo_);
	//VECTOR pos = transform_.pos;

	// �ʒu�̐ݒ�
	SetPosPlayingEffekseer3DEffect(
		effektHappyPlayId_,
		pos.x,
		pos.y,
		pos.z);

	SetReaction(REACTION::NONE);
}
