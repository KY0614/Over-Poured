#pragma once
#include <memory>
#include "../ActorBase.h"

class AnimationController;

class CustomerBase : public ActorBase
{
public:

	static constexpr VECTOR COUNTER_FRONT_POS = {221.0f, 0.0f, 271.0f};
	static constexpr VECTOR CUSTOMER_POS = {-139.0f, 0.0f, 271.0f};
	static constexpr float COUNTER_FRONT_POS_X = 221.0f;

	//回転完了までの時間
	static constexpr float TIME_ROT = 1.0f;

	enum class TYPE
	{
		NONE,
		HOT,
		ICE
	};

	enum class STATE
	{
		IDLE,
		WALK,
		WAIT,
	};

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

	virtual void Init(VECTOR pos);
	//virtual void Init(void)override = 0;
	virtual void Update(void)override;
	virtual void Draw(void)override = 0;

	//一定間隔だけ進む
	void Move(void);

	void RotateY(void);

	/// <summary>
	/// カウンター前の球体との当たり判定
	/// </summary>
	/// <returns>true:当たっている , false:当たっていない</returns>
	bool CollisionCounter(void);

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

	virtual void SetParam(void) = 0;

	virtual void InitAnimation(void) = 0;

	virtual void UpdateCol(void) = 0;
private:

	//回転
	Quaternion customerRotY_;
	Quaternion goalQuaRot_;
	float stepRotTime_;
};

