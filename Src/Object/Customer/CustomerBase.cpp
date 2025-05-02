#include<DxLib.h>
#include "../../Manager/Generic/SceneManager.h"
#include "../Common/AnimationController.h"
#include "../../Utility/AsoUtility.h"
#include "CustomerBase.h"

CustomerBase::CustomerBase(void)
{
	animationController_ = nullptr;
	type_ = TYPE::NONE;
	state_ = STATE::IDLE;

	isMove_ = false;
	stepRotTime_ = 0.0f;
}

void CustomerBase::Init(VECTOR pos)
{
	transform_.scl = AsoUtility::VECTOR_ONE;
	transform_.pos = CUSTOMER_POS;
	transform_.pos = pos;
	transform_.quaRot = Quaternion();
	transform_.quaRotLocal =
		Quaternion::Euler({ 0.0f, AsoUtility::Deg2RadF(-90.0f), 0.0f });

	transform_.Update();

	SetParam();
	InitAnimation();
	isMove_ = false;
}

void CustomerBase::Update(void)
{
	transform_.Update();
	animationController_->Update();
	RotateY();

	//�d�͕����ɉ����ĉ�]������
	transform_.quaRot = Quaternion::Quaternion();
	transform_.quaRot = transform_.quaRot.Mult(customerRotY_);
}

void CustomerBase::Move(void)
{
	//if (transform_.pos.x < transform_.targetPos.x)
	//{
	//	isMove_ = true;
	//	transform_.pos.x++;
	//}
	//else {
	//	isMove_ = false;
	//}
	transform_.pos.x += 2.0f;
}

void CustomerBase::RotateY(void)
{
	stepRotTime_ -= SceneManager::GetInstance().GetDeltaTime();

	//��]�̋��ʕ��
	customerRotY_ = Quaternion::Slerp(
		customerRotY_, goalQuaRot_, (TIME_ROT - stepRotTime_) / TIME_ROT);
}

bool CustomerBase::CollisionCounter(void)
{
	VECTOR spherePos = { 221.0f, 0.0f, 271.0f };

	if (AsoUtility::IsHitSpherePoint(spherePos, 30, GetTransform().pos))
	{
		//�������Ă�����true��Ԃ�
		return true;
	}
	return false;
}

bool CustomerBase::CheckCounterToCustomer(void)
{
	VECTOR spherePos = { 221.0f, 0.0f, 271.0f };
	if (GetTransform().pos.x < spherePos.x)
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
