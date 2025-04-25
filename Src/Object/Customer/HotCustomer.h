//#pragma once
#include "../Customer/CustomerBase.h"

class HotCustomer : public CustomerBase
{
public:

	//コンストラクタ
	HotCustomer(void);

	//デストラクタ
	~HotCustomer(void) = default;

	//void Init(void)override;
	void Update(void) override;
	void Draw(void) override;

	void SetParam(void) override;


protected:


private:


};

