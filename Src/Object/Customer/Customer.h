//#pragma once
#include "../ActorBase.h"
#include "../Customer/CustomerBase.h"

class Customer : public CustomerBase 
{
public:
	//コンストラクタ
	Customer(void);

	//デストラクタ
	~Customer(void);

	void Init(void)override;
	void Update(void) override;
	void Draw(void) override;

private:
	void SetParam(void) override;

};

