//#pragma once
#include "../Customer/CustomerBase.h"

class HotCustomer : public CustomerBase
{
public:

	//コンストラクタ
	HotCustomer(void);

	//デストラクタ
	~HotCustomer(void);

	void Draw(void) override;

protected:

	void SetParam(void) override;

	virtual void InitAnimation(void) override;

private:


};

