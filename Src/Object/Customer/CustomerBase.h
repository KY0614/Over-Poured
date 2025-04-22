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

	/// <summary>
	/// 客の種類を設定する
	/// </summary>
	/// <param name="type">設定する種類</param>
	void SetType(TYPE type) { type_ = type; }

	//一定間隔だけ進む
	void Move(void);

	virtual void SetParam(void) = 0;

protected:

	TYPE type_;

	STATE state_;

private:

};

