#pragma once
#include "../Customer/CustomerBase.h"


class IceCustomer : public CustomerBase
{
public:

	IceCustomer(void);
	~IceCustomer(void) = default;

	void Draw(void) override;

	
protected:
	void SetParam(void) override;

	void InitAnimation(void) override;

	void UpdateCol(void)override;

private:

};

