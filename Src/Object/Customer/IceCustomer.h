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

	virtual void InitAnimation(void) override;
private:

};

