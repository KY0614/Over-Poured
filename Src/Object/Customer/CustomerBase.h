#pragma once
#include <memory>
#include "../ActorBase.h"

class AnimationController;

class CustomerBase : public ActorBase
{
public:
	//���q�̏����ʒu
	static constexpr VECTOR CUSTOMER_POS = {-139.0f, 0.0f, 271.0f};		//��ʍ��̂ق�

	//�A�j���[�V�����̑��x
	static constexpr float IDLE_ANIM_SPEED = 20.0f;		//�ҋ@�A�j���[�V�����̑��x
	static constexpr float WALK_ANIM_SPEED = 30.0f;		//���s�A�j���[�V�����̑��x

	//���q�̎�ށi�^�C�v�j
	enum class TYPE
	{
		NONE,	//�Ȃ�
		HOT,	//�z�b�g�̌�����
		ICE,	//�A�C�X�̌�����
	};

	//���q�̏��
	enum class STATE
	{
		IDLE,	//�ҋ@���
		WALK,	//���s���
	};

	//���q�̔���
	enum class REACTION
	{	
		NONE,	//�Ȃ�
		GOOD,	//�ǂ�����
		SOSO,	//���ʂ̔���
		BAD,	//��������
	};

	//�R���X�g���N�^
	CustomerBase(void);

	//�f�X�g���N�^
	~CustomerBase(void) = default;

	/// <summary>
	/// ����������
	/// </summary>
	/// <param name="pos">�ݒ肷����W</param>
	virtual void Init(const VECTOR pos);

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

	//�ݒ�֐�---------------------------------------------------

	/// <summary>
	/// �q�̎�ނ�ݒ肷��
	/// </summary>
	/// <param name="type">�ݒ肷����</param>
	void SetType(const TYPE type) { type_ = type; }

	/// <summary>
	/// X���W��ݒ�
	/// </summary>
	/// <param name="posX">X���W</param>
	void SetPosX(const float posX) { transform_.pos.x = posX; }

	/// <summary>
	/// ���q�̏�Ԃ�ݒ肷��
	/// </summary>
	/// <param name="state">�ݒ肷����</param>
	void SetState(const STATE state) { state_ = state; }

	/// <summary>
	/// ���q�̔�����ݒ肷��
	/// </summary>
	/// <param name="reaction">�ݒ肷�锽��</param>
	void SetReaction(const REACTION reaction) { reaction_ = reaction; }

	/// <summary>
	/// �ڕW�̉�]�p�x�i���W�A���j��ݒ肵�܂��B
	/// </summary>
	/// <param name="rotRad">�ݒ肷���]�p�x�i���W�A���P�ʁj</param>
	void SetGoalRotate(const double rotRad);

	/// <summary>
	/// ���q���\���ɂ���
	/// </summary>
	/// <param name="">false�ɂ��Ĕ�\��</param>
	void IsNotVisible(void) { isVisible_ = false; }

	//--------------------------------------------------------------------------
	//�擾�֐�------------------------------------------------------------------

	/// <summary>
	/// �J�E���^�[�O�̋��̂Ƃ̓����蔻��
	/// </summary>
	/// <returns>true:�������Ă��� , false:�������Ă��Ȃ�</returns>
	bool CollisionCounter(void);

	/// <summary>
	/// ���q���J�E���^�[�̑O�ɂ��邩�ǂ������m�F����
	/// </summary>
	/// <param name=""></param>
	/// <returns>true�F�J�E���^�[�̈ʒu���z���Ă��� false�F�z���Ă��Ȃ�</returns>
	bool CheckCounterToCustomer(void);

	/// <summary>
	/// ���q�̍��W���擾����
	/// </summary>
	/// <returns>���q�̍��W</returns>
	VECTOR GetPos(void)const { return transform_.pos; }

protected:

	//�A�j���[�V����
	std::unique_ptr<AnimationController> animationController_;

	//���q�̎��
	TYPE type_;

	//���q�̏��
	STATE state_;

	//���q�̔���
	REACTION reaction_;

	//�\�����邩�ǂ���
	bool isVisible_;

	/// <summary>
	/// �p�����[�^��ݒ肷��
	/// </summary>
	/// <param name="">���q���Ƃ̃p�����[�^�[</param>
	virtual void SetParam(void) = 0;

	/// <summary>
	/// �A�j���[�V�����̏���������
	/// </summary>
	virtual void InitAnimation(void) = 0;

private:

	//���q��Y���̉�]�p�x
	Quaternion customerRotY_;

	//��]�p�x�̖ڕW�l
	Quaternion goalQuaRot_;

	//��]����
	float stepRotTime_;

	//���A�N�V�����FHAPPY�̃G�t�F�N�g
	int effektHappyResId_;	//�G�t�F�N�g�̃��\�[�XID
	int effektHappyPlayId_;	//�G�t�F�N�g�̍Đ�ID

	//���A�N�V�����FSOSO�̃G�t�F�N�g
	int effektSosoResId_;	//�G�t�F�N�g�̃��\�[�XID
	int effektSosoPlayId_;	//�G�t�F�N�g�̍Đ�ID

	//���A�N�V�����FBAD�̃G�t�F�N�g
	int effektBadResId_;	//�G�t�F�N�g�̃��\�[�XID
	int effektBadPlayId_;	//�G�t�F�N�g�̍Đ�ID

	//�`�F�X�g�̃t���[���ԍ��i�G�t�F�N�g�̍��W�p)
	int chestFrmNo_;

	/// <summary>
	/// Y���𒆐S�ɉ�]����
	/// </summary>
	void RotateY(void);

	/// <summary>
	/// ��Ԃ��Ƃ̃A�j���[�V��������
	/// </summary>
	/// <param name="">�A�j���[�V�����Đ�</param>
	void StateAnimation(void);

	/// <summary>
	/// ���A�N�V�������Ƃ̏���
	/// </summary>
	/// <param name=""></param>
	void StateReaction(void);

	/// <summary>
	/// ���A�N�V�������̃G�t�F�N�g�̍Đ�
	/// </summary>
	/// <param name="playId">�G�t�F�N�g�̍Đ�Id</param>
	/// <param name="resId">�G�t�F�N�g�̃��\�[�XID</param>
	void ReactionEffektPlay(int playId, int resId);
};

