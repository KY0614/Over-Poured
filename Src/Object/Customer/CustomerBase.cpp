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
	isMovedCounter_ = false;
}

void CustomerBase::Init(void)
{
	transform_.scl = AsoUtility::VECTOR_ONE;
	transform_.pos = CUSTOMER_POS;
	transform_.targetPos.x = transform_.pos.x + 90.0f;
	transform_.quaRot = Quaternion();
	transform_.quaRotLocal =
		Quaternion::Euler({ 0.0f, AsoUtility::Deg2RadF(-90.0f), 0.0f });

	transform_.Update();

	SetParam();
	InitAnimation();
	isMovedCounter_ = false;
}

void CustomerBase::Update(void)
{
	transform_.Update();
	animationController_->Update();
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
	transform_.pos.x++;
}

void CustomerBase::Move2Counter(void)
{
}

bool CustomerBase::CollisionCounter(void)
{
	VECTOR spherePos = { 221.0f, 0.0f, 271.0f };

	if (AsoUtility::IsHitSpherePoint(spherePos, 30, GetTransform().pos))
	{
		//“–‚½‚Á‚Ä‚¢‚½‚çtrue‚ð•Ô‚·
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

	//‰ñ“]‚Ì‹…–Ê•âŠÔ
	playerRotY_ = Quaternion::Slerp(
		playerRotY_, goalQuaRot_, (TIME_ROT - stepRotTime_) / TIME_ROT);
}
