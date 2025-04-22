//#pragma once
#include "../ActorBase.h"

class CustomerBase : public ActorBase
{
public:
	enum class TYPE
	{
		NONE,
		HOT,
		ICE
	};

	//�R���X�g���N�^
	CustomerBase(void);

	//�f�X�g���N�^
	~CustomerBase(void) = default;

	//���
	virtual void Destroy(void);

	void Init(void) override;
	void Update(void) override;
	void Draw(void) override;

	void SetType(TYPE type) { type_ = type; }

	void Move(void);

protected:

	TYPE type_;

	virtual void SetParam(void) = 0;

private:

};

