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

	enum class STATE
	{
		IDLE,
		WALK,
		WAIT,
		GOOD,
		BAD,
	};

	//�R���X�g���N�^
	CustomerBase(void);

	//�f�X�g���N�^
	~CustomerBase(void) = default;

	void Init(void) override;
	void Update(void) override;
	void Draw(void) override;

	/// <summary>
	/// �q�̎�ނ�ݒ肷��
	/// </summary>
	/// <param name="type">�ݒ肷����</param>
	void SetType(TYPE type) { type_ = type; }

	//���Ԋu�����i��
	void Move(void);


protected:

	TYPE type_;

	virtual void SetParam(void) = 0;

private:

};

