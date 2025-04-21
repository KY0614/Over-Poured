//#pragma once
#include "../ActorBase.h"
#include "../Customer/CustomerBase.h"

class Customer : public CustomerBase 
{
public:
	//�R���X�g���N�^
	Customer(void);

	//�f�X�g���N�^
	~Customer(void);

	void Init(void)override;
	void Update(void) override;
	void Draw(void) override;

private:
	void SetParam(void) override;

};

