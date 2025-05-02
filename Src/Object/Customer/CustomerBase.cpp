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

	//d—Í•ûŒü‚É‰ˆ‚Á‚Ä‰ñ“]‚³‚¹‚é
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

	//‰ñ“]‚Ì‹…–Ê•âŠÔ
	customerRotY_ = Quaternion::Slerp(
		customerRotY_, goalQuaRot_, (TIME_ROT - stepRotTime_) / TIME_ROT);
}

bool CustomerBase::CollisionCounter(void)
{
	VECTOR spherePos = { 221.0f, 0.0f, 271.0f };

	if (AsoUtility::IsHitSpherePoint(spherePos, 30, GetTransform().pos))
	{
		//“–‚½‚Á‚Ä‚¢‚½‚çtrue‚ğ•Ô‚·
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

	//Œ»İİ’è‚³‚ê‚Ä‚¢‚é‰ñ“]‚Æ‚ÌŠp“x·‚ğæ‚é
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
