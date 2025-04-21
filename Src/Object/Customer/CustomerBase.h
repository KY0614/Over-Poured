//#pragma once
#include "../ActorBase.h"

class CustomerBase : public ActorBase
{
	enum class TYPE
	{
		HOT,
		ICE
	};

	//�R���X�g���N�^
	CustomerBase(void);

	//�f�X�g���N�^
	~CustomerBase(void);

	void Init(void) override;
	void Update(void) override;
	void Draw(void) override;

protected:
	TYPE type_;

	virtual void SetParam(void) = 0;

private:

};

