#pragma once
#include "../ActorBase.h"

class CustomerBase : public ActorBase
{
public:

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
		GOOD,
		BAD,
	};

	//コンストラクタ
	CustomerBase(void);

	//デストラクタ
	~CustomerBase(void) = default;

	virtual void Init(void)override = 0;
	virtual void Update(void)override = 0;
	virtual void Draw(void)override = 0;

	//一定間隔だけ進む
	void Move(void);

	/// <summary>
	/// 客の種類を設定する
	/// </summary>
	/// <param name="type">設定する種類</param>
	void SetType(TYPE type) { type_ = type; }

	/// <summary>
	/// 位置を設定
	/// </summary>
	/// <param name="pos">位置</param>
	void SetPos(VECTOR pos) { transform_.pos = pos; }

	VECTOR GetPos(void) { return transform_.pos; }

	virtual void SetParam(void) = 0;

protected:

	TYPE type_;

	STATE state_;



private:

};

