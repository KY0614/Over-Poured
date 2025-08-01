#include<DxLib.h>
#include "../../Manager/Generic/SceneManager.h"
#include "../Common/AnimationController.h"
#include "../../Utility/AsoUtility.h"
#include "CustomerBase.h"

namespace
{
	const VECTOR CUSTOMER_SCALE = { 0.7f, 0.7f, 0.7f };	//お客の大きさ
	const VECTOR COUNTER_POS = { 221.0f, 0.0f, 271.0f };//カウンターの位置
	const float CUSTOMER_ROT_Y = -90.0f; // 顧客の初期Y軸回転角度
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
	//モデル情報初期化
	transform_.scl = CUSTOMER_SCALE;	//大きさ
	transform_.pos = pos;				//位置を引数で指定
	transform_.quaRot = Quaternion();	//回転は初期化
	transform_.quaRotLocal =			//ローカル回転設定
		Quaternion::Euler({ 0.0f, AsoUtility::Deg2RadF(CUSTOMER_ROT_Y), 0.0f });
	//モデル情報を更新
	transform_.Update();
	//お客のパラメーター設定
	SetParam();
	//アニメーション初期化
	InitAnimation();
}

void CustomerBase::Update(void)
{
	transform_.Update();
	animationController_->Update();

	//状態ごとのアニメーション
	StateAnimation();
	//Y軸回転
	RotateY();

	//Y軸回転を反映
	transform_.quaRot = Quaternion::Quaternion();
	transform_.quaRot = transform_.quaRot.Mult(customerRotY_);
}

void CustomerBase::Move(void)
{
	//右に移動させる
	transform_.pos.x += MOVE_SPEED;
}

void CustomerBase::RotateY(void)
{
	//回転にかかる時間を経過時で減らす
	stepRotTime_ -= SceneManager::GetInstance().GetDeltaTime();

	//回転の球面補間
	customerRotY_ = Quaternion::Slerp(
		customerRotY_, goalQuaRot_, (TIME_ROT - stepRotTime_) / TIME_ROT);
}

bool CustomerBase::CollisionCounter(void)
{
	if (AsoUtility::IsHitSpherePoint(COUNTER_POS, 30, GetTransform().pos) && isVisible_)
	{
		//当たっていたらtrueを返す
		return true;
	}
	return false;
}

bool CustomerBase::CheckCounterToCustomer(void)
{
	//お客がカウンターの位置を超えそうだったらtrueを返す
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

	//現在設定されている回転との角度差を取る
	double angleDiff = Quaternion::Angle(axis, goalQuaRot_);

	//しきい値
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
	//状態ごとによるアニメーションの切り替え
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
