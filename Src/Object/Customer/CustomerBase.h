#pragma once
#include <memory>
#include "../ActorBase.h"

class AnimationController;

class CustomerBase : public ActorBase
{
public:
	//���q�̏����ʒu
	static constexpr VECTOR CUSTOMER_POS = {-139.0f, 0.0f, 271.0f};

	//��]�����܂ł̎���
	static constexpr float TIME_ROT = 1.0f;

	//���q�̎�ށi�^�C�v�j
	enum class TYPE
	{
		NONE,
		HOT,
		ICE
	};

	//���q�̏��
	enum class STATE
	{
		IDLE,
		WALK,
	};

	//���q�̔���
	enum class REACTION
	{
		NONE,
		GOOD,
		SOSO,
		BAD,
	};

	//�R���X�g���N�^
	CustomerBase(void);

	//�f�X�g���N�^
	~CustomerBase(void) = default;

	/// <summary>
	/// ����������
	/// </summary>
	/// <param name="pos">�ݒ肷����W</param>
	virtual void Init(VECTOR pos);

	/// <summary>
	/// �X�V����
	/// </summary>
	virtual void Update(void)override;

	/// <summary>
	/// �`�揈��
	/// </summary>
	virtual void Draw(void)override = 0;

	//���Ԋu�����i��
	
	/// <summary>
	/// �ړ�����
	/// </summary>
	/// <param name="">�E�Ɉ��Ԋu�ňړ�����</param>
	void Move(void);

	/// <summary>
	/// Y���𒆐S�ɉ�]����
	/// </summary>
	void RotateY(void);

	/// <summary>
	/// �J�E���^�[�O�̋��̂Ƃ̓����蔻��
	/// </summary>
	/// <returns>true:�������Ă��� , false:�������Ă��Ȃ�</returns>
	bool CollisionCounter(void);

	/// <summary>
	/// ���q���J�E���^�[�̑O�ɂ��邩�ǂ������m�F����
	/// </summary>
	/// <param name=""></param>
	/// <returns>true�F�J�E���^�[�̈ʒu�𒴂��Ă���ꍇ�Afalse�F�����łȂ��ꍇ</returns>
	bool CheckCounterToCustomer(void);

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

	void SetState(STATE state) { state_ = state; }
	void SetReaction(REACTION reaction) { reaction_ = reaction; }

	void IsVisible(void) { isVisible_ = false; }
	bool GetIsVisible(void) { return isVisible_; }

	STATE GetState(void) { return state_; }

	VECTOR GetPos(void) { return transform_.pos; }

	//��]
	void SetGoalRotate(double rotRad);
	void Rotate(void);

protected:

	//�A�j���[�V����
	std::unique_ptr<AnimationController> animationController_;

	TYPE type_;

	STATE state_;
	REACTION reaction_;

	COLOR_F color_;

	//�\�����邩�ǂ���
	bool isVisible_;

	virtual void SetParam(void) = 0;

	virtual void InitAnimation(void) = 0;

	virtual void UpdateCol(void) = 0;

private:

	//��]
	Quaternion customerRotY_;
	Quaternion goalQuaRot_;
	float stepRotTime_;

	void StateAnimation(void);
};

