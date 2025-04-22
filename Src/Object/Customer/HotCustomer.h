//#pragma once
#include "../Customer/CustomerBase.h"

class HotCustomer : public CustomerBase 
{
public:

	//�R���X�g���N�^
	HotCustomer(void);

	//�f�X�g���N�^
	~HotCustomer(void) = default;

	void Init(void)override;
	void Update(void) override;
	void Draw(void) override;

private:

	void SetParam(void) override;

};

