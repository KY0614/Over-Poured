#pragma once
#include <memory>
#include "../ActorBase.h"

class AnimationController;

class CustomerBase : public ActorBase
{
public:

	static constexpr VECTOR COUNTER_FRONT_POS = {221.0f, 0.0f, 271.0f};
	static constexpr VECTOR CUSTOMER_POS = {-139.0f, 0.0f, 271.0f};
	static constexpr float COUNTER_FRONT_POS_X = 221.0f;

	//��]�����܂ł̎���
	static constexpr float TIME_ROT = 1.0f;

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

	virtual void Init(void)override;
	virtual void Update(void)override;
	virtual void Draw(void)override = 0;

	//���Ԋu�����i��
	void Move(void);

	void Move2Counter(void);

	/// <summary>
	/// �J�E���^�[�O�̋��̂Ƃ̓����蔻��
	/// </summary>
	/// <returns>true:�������Ă��� , false:�������Ă��Ȃ�</returns>
	bool CollisionCounter(void);

	/// <summary>
	/// �q�̎�ނ�ݒ肷��
	/// </summary>
	/// <param name="type">�ݒ肷����</param>
	void SetType(TYPE type) { type_ = type; }

	/// <summary>
	/// �ʒu��ݒ�
	/// </summary>
	/// <param name="pos">�ʒu</param>
	void SetPosX(float x) { transform_.pos.x = x; }
	void SetPos(VECTOR pos) { transform_.pos = pos; }

	void SetRotY(float rotY) { transform_.rot.y = rotY; }

	void SetIsMove(bool isMove) { isMove_ = isMove; }
	void IsMove(void) { isMove_ = true; }

	VECTOR GetPos(void) { return transform_.pos; }
	bool GetIsMove(void) { return isMove_; }

	//��]
	void SetGoalRotate(double rotRad);
	void Rotate(void);

protected:

	//�A�j���[�V����
	std::unique_ptr<AnimationController> animationController_;

	TYPE type_;

	STATE state_;

	virtual void SetParam(void) = 0;

	virtual void InitAnimation(void) = 0;

private:

	//��]
	Quaternion playerRotY_;
	Quaternion goalQuaRot_;
	float stepRotTime_;

	bool isMove_;
};

