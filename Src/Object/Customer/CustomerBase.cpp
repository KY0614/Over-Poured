#include<DxLib.h>
#include<EffekseerForDXLib.h>
#include "../../Manager/Generic/SceneManager.h"
#include "../../Manager/Generic/ResourceManager.h"
#include "../Common/AnimationController.h"
#include "../../Utility/CommonUtility.h"
#include "CustomerBase.h"

namespace//このcpp内でしか使わない定数
{
	const VECTOR CUSTOMER_SCALE = { 0.7f, 0.7f, 0.7f };	//お客の大きさ

	const VECTOR COUNTER_POS = { 221.0f, 0.0f, 271.0f };//カウンターの位置

	const float CUSTOMER_ROT_Y = -90.0f;	//お客の初期Y軸回転角度

	const float MOVE_SPEED = 1.5f;			//お客の移動速度

	const float COUNTER_RADIUS = 30.0f;		//カウンターの球体当たり判定の半径

	const float ROT_TIME = 1.0f;			//回転完了までの時間

	const float EFFEKT_SCALE = 30.0f;		//エフェクトの大きさ
	const float EFFEKT_SPEED = 2.0f;		//エフェクトの再生速度
}

CustomerBase::CustomerBase(void)
{
	animationController_ = nullptr;
	type_ = TYPE::NONE;
	state_ = STATE::IDLE;
	reaction_ = REACTION::NONE;

	isVisible_ = false;

	stepRotTime_ = -1.0f;

	effektHappyResId_ = -1;
	effektHappyPlayId_ = -1;
	effektBadResId_ = -1;
	effektBadPlayId_ = -1;
	effektSosoResId_ = -1;
	effektSosoPlayId_ = -1;
	chestFrmNo_ = -1;
}

void CustomerBase::Init(const VECTOR pos)
{
	//お客のパラメーター設定
	SetParam();

	//アニメーション初期化
	InitAnimation();

	//モデル情報初期化
	transform_.scl = CUSTOMER_SCALE;	//大きさ
	transform_.pos = pos;				//位置を引数で指定
	transform_.quaRot = Quaternion();	//回転は初期化
	transform_.quaRotLocal =			//ローカル回転設定
		Quaternion::Euler({ 0.0f, CommonUtility::Deg2RadF(CUSTOMER_ROT_Y), 0.0f });
	//モデル情報を更新
	transform_.Update();

	//星のエフェクトのリソース読み込み
	effektHappyResId_ = ResourceManager::GetInstance().Load(
		ResourceManager::SRC::HAPPY_STAR).handleId_;

	//紫のぐるぐるエフェクトのリソース読み込み
	effektBadResId_ = ResourceManager::GetInstance().Load(
		ResourceManager::SRC::GURU).handleId_;

	//緑のオーブエフェクトのリソース読み込み
	effektSosoResId_ = ResourceManager::GetInstance().Load(
		ResourceManager::SRC::ORB).handleId_;

	//お客のモデルの胸の位置のフレーム番号を取得
	chestFrmNo_ = MV1SearchFrame(transform_.modelId, L"mixamorig:Head");
}

void CustomerBase::Update(void)
{
	//モデル情報更新
	transform_.Update();
	//アニメーション更新
	animationController_->Update();

	//状態ごとのアニメーション
	StateAnimation();
	StateReaction();
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

void CustomerBase::SetGoalRotate(double rotRad)
{ 
	//回転軸をY軸に設定して回転角度を設定
	Quaternion axis =
		Quaternion::AngleAxis(
			rotRad, CommonUtility::AXIS_Y);

	//現在設定されている回転との角度差を取る
	double angleDiff = Quaternion::Angle(axis, goalQuaRot_);

	//角度差が0.1以上なら回転時間を設定
	if (angleDiff > 0.1)
	{
		stepRotTime_ = ROT_TIME;
	}
	//目標の回転を設定
	goalQuaRot_ = axis;
}

const bool& CustomerBase::CollisionCounter(void)
{
	//表示中のお客がカウンターの球体当たり判定に当たっているか
	if (CommonUtility::IsHitSpherePoint(COUNTER_POS, COUNTER_RADIUS,
		GetTransform().pos) && isVisible_)
	{
		//当たっていたらtrueを返す
		return true;
	}
	return false;
}

const bool& CustomerBase::CheckCounterToCustomer(void)
{
	//お客がカウンターの位置を超えそうだったらtrueを返す
	if (GetTransform().pos.x > COUNTER_POS.x)
	{
		return true;
	}
	return false;
}

void CustomerBase::RotateY(void)
{
	//回転にかかる時間を経過時で減らす
	stepRotTime_ -= SceneManager::GetInstance().GetDeltaTime();

	//回転の球面補間
	customerRotY_ = Quaternion::Slerp(
		customerRotY_, goalQuaRot_, (ROT_TIME - stepRotTime_) / ROT_TIME);
}

void CustomerBase::StateAnimation(void)
{
	//状態ごとによるアニメーションの切り替え
	switch (state_)
	{
	case CustomerBase::STATE::IDLE:	//待機状態
		animationController_->Play((int)STATE::IDLE);
		break;

	case CustomerBase::STATE::WALK:	//歩行状態
		animationController_->Play((int)STATE::WALK);
		break;
	default:
		break;
	}
}

void CustomerBase::StateReaction(void)
{
	//状態ごとによるアニメーションの切り替え
	switch (reaction_)
	{
	case CustomerBase::REACTION::NONE:
		break;

	case CustomerBase::REACTION::BAD:
		//BAD用のエフェクト再生
		ReactionEffektPlay(effektBadPlayId_, effektBadResId_);
		break;

	case CustomerBase::REACTION::SOSO:
		//SOSO用のエフェクト再生
		ReactionEffektPlay(effektSosoPlayId_, effektSosoResId_);
		break;

	case CustomerBase::REACTION::GOOD:
		//GOOD用のエフェクト再生
		ReactionEffektPlay(effektHappyPlayId_, effektHappyResId_);
		break;

	default:
		break;
	}
}

void CustomerBase::ReactionEffektPlay(int playId, int resId)
{
	//再生Idを取得
	playId = PlayEffekseer3DEffect(resId);

	//再生速度の設定(少し早めに設定）
	SetSpeedPlayingEffekseer3DEffect(playId, EFFEKT_SPEED);

	//大きさの設定
	SetScalePlayingEffekseer3DEffect(
		playId,
		EFFEKT_SCALE,
		EFFEKT_SCALE,
		EFFEKT_SCALE
	);

	//エフェクトの位置をお客の胸の位置に設定
	VECTOR pos = MV1GetFramePosition(transform_.modelId, chestFrmNo_);
	SetPosPlayingEffekseer3DEffect(
		playId,
		pos.x,
		pos.y,
		pos.z);

	//1回だけ再生したいのでREACTIONをNONEに戻す
	SetReaction(REACTION::NONE);
}