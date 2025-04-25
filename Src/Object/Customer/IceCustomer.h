#pragma once
#include "../Customer/CustomerBase.h"

class IceCustomer : public CustomerBase
{
public:

	IceCustomer(void);
	~IceCustomer(void) = default;

	//void Init(void)override;
	void Update(void) override;
	void Draw(void) override;

	void SetParam(void) override;


protected:


private:

};

