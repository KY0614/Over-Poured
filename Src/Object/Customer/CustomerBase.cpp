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
	reaction_ = REACTION::NONE;

	isVisible_ = false;

	stepRotTime_ = 0.0f;
}

void CustomerBase::Init(VECTOR pos)
{
	transform_.scl = {0.7f,0.7f,0.7f};
	transform_.pos = CUSTOMER_POS;
	transform_.pos = pos;
	transform_.quaRot = Quaternion();
	transform_.quaRotLocal =
		Quaternion::Euler({ 0.0f, AsoUtility::Deg2RadF(-90.0f), 0.0f });

	transform_.Update();

	SetParam();
	InitAnimation();
}

void CustomerBase::Update(void)
{
	transform_.Update();
	animationController_->Update();

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

	RotateY();

	//‰ñ“]‚³‚¹‚é
	transform_.quaRot = Quaternion::Quaternion();
	transform_.quaRot = transform_.quaRot.Mult(customerRotY_);
}

void CustomerBase::Move(void)
{
	transform_.pos.x += 1.5f;
}

void CustomerBase::RotateY(void)
{
	stepRotTime_ -= SceneManager::GetInstance().GetDeltaTime();

	//‰ñ“]‚Ì‹…–Ê•âŠÔ
	customerRotY_ = Quaternion::Slerp(
		customerRotY_, goalQuaRot_, (TIME_ROT - stepRotTime_) / TIME_ROT);
}

bool CustomerBase::CollisionCounter(void)
{
	VECTOR spherePos = { 221.0f, 0.0f, 271.0f };

	if (AsoUtility::IsHitSpherePoint(spherePos, 30, GetTransform().pos) && isVisible_)
	{
		//“–‚½‚Á‚Ä‚¢‚½‚çtrue‚ð•Ô‚·
		return true;
	}
	return false;
}

bool CustomerBase::CheckCounterToCustomer(void)
{
	VECTOR spherePos = { 221.0f, 0.0f, 271.0f };
	if (GetTransform().pos.x > spherePos.x)
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

	//Œ»ÝÝ’è‚³‚ê‚Ä‚¢‚é‰ñ“]‚Æ‚ÌŠp“x·‚ðŽæ‚é
	double angleDiff = Quaternion::Angle(axis, goalQuaRot_);

	//‚µ‚«‚¢’l
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
