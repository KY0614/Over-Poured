//#pragma once
#include "../Customer/CustomerBase.h"

class HotCustomer : public CustomerBase
{
public:

	//�R���X�g���N�^
	HotCustomer(void);

	//�f�X�g���N�^
	~HotCustomer(void);

	void Draw(void) override;

protected:

	void SetParam(void) override;

	void InitAnimation(void) override;

	void UpdateCol(void)override;

private:


};

