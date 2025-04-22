//#pragma once
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

	void Init(void) override;
	void Update(void) override;
	void Draw(void) override;

	/// <summary>
	/// 客の種類を設定する
	/// </summary>
	/// <param name="type">設定する種類</param>
	void SetType(TYPE type) { type_ = type; }

	//一定間隔だけ進む
	void Move(void);


protected:

	TYPE type_;

	virtual void SetParam(void) = 0;

private:

};

