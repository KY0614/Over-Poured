//#pragma once
#include "../ActorBase.h"

class CustomerBase : public ActorBase
{
	enum class TYPE
	{
		HOT,
		ICE
	};

	//コンストラクタ
	CustomerBase(void);

	//デストラクタ
	~CustomerBase(void);

	void Init(void) override;
	void Update(void) override;
	void Draw(void) override;

protected:
	TYPE type_;

	virtual void SetParam(void) = 0;

private:

};

