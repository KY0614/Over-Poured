#pragma once
#include <memory>
#include "../ActorBase.h"

class AnimationController;

class CustomerBase : public ActorBase
{
public:
	//お客の初期位置
	static constexpr VECTOR CUSTOMER_POS = {-139.0f, 0.0f, 271.0f};

	//回転完了までの時間
	static constexpr float TIME_ROT = 1.0f;

	//お客の種類（タイプ）
	enum class TYPE
	{
		NONE,
		HOT,
		ICE
	};

	//お客の状態
	enum class STATE
	{
		IDLE,
		WALK,
	};

	//お客の反応
	enum class REACTION
	{
		NONE,
		GOOD,
		SOSO,
		BAD,
	};

	//コンストラクタ
	CustomerBase(void);

	//デストラクタ
	~CustomerBase(void) = default;

	/// <summary>
	/// 初期化処理
	/// </summary>
	/// <param name="pos">設定する座標</param>
	virtual void Init(VECTOR pos);

	/// <summary>
	/// 更新処理
	/// </summary>
	virtual void Update(void)override;

	/// <summary>
	/// 描画処理
	/// </summary>
	virtual void Draw(void)override = 0;

	//一定間隔だけ進む
	
	/// <summary>
	/// 移動処理
	/// </summary>
	/// <param name="">右に一定間隔で移動する</param>
	void Move(void);

	/// <summary>
	/// Y軸を中心に回転する
	/// </summary>
	void RotateY(void);

	/// <summary>
	/// カウンター前の球体との当たり判定
	/// </summary>
	/// <returns>true:当たっている , false:当たっていない</returns>
	bool CollisionCounter(void);

	/// <summary>
	/// お客がカウンターの前にいるかどうかを確認する
	/// </summary>
	/// <param name=""></param>
	/// <returns>true：カウンターの位置を超えている場合、false：そうでない場合</returns>
	bool CheckCounterToCustomer(void);

	/// <summary>
	/// 客の種類を設定する
	/// </summary>
	/// <param name="type">設定する種類</param>
	void SetType(TYPE type) { type_ = type; }

	/// <summary>
	/// 位置を設定
	/// </summary>
	/// <param name="pos">位置</param>
	void SetPosX(float x) { transform_.pos.x = x; }
	void SetPos(VECTOR pos) { transform_.pos = pos; }

	void SetRotY(float rotY) { transform_.rot.y = rotY; }

	void SetState(STATE state) { state_ = state; }
	void SetReaction(REACTION reaction) { reaction_ = reaction; }

	void IsVisible(void) { isVisible_ = false; }
	bool GetIsVisible(void) { return isVisible_; }

	STATE GetState(void) { return state_; }

	VECTOR GetPos(void) { return transform_.pos; }

	//回転
	void SetGoalRotate(double rotRad);
	void Rotate(void);

protected:

	//アニメーション
	std::unique_ptr<AnimationController> animationController_;

	TYPE type_;

	STATE state_;
	REACTION reaction_;

	COLOR_F color_;

	//表示するかどうか
	bool isVisible_;

	virtual void SetParam(void) = 0;

	virtual void InitAnimation(void) = 0;

	virtual void UpdateCol(void) = 0;

private:

	//回転
	Quaternion customerRotY_;
	Quaternion goalQuaRot_;
	float stepRotTime_;

	void StateAnimation(void);
};

