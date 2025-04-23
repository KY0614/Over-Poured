#pragma once
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

	virtual void Init(void)override = 0;
	virtual void Update(void)override = 0;
	virtual void Draw(void)override = 0;

	//���Ԋu�����i��
	void Move(void);

	/// <summary>
	/// �q�̎�ނ�ݒ肷��
	/// </summary>
	/// <param name="type">�ݒ肷����</param>
	void SetType(TYPE type) { type_ = type; }

	/// <summary>
	/// �ʒu��ݒ�
	/// </summary>
	/// <param name="pos">�ʒu</param>
	void SetPos(VECTOR pos) { transform_.pos = pos; }

	VECTOR GetPos(void) { return transform_.pos; }

	virtual void SetParam(void) = 0;

protected:

	TYPE type_;

	STATE state_;



private:

};

