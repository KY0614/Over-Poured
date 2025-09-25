#pragma once
#include <memory>
#include <vector>
#include <map>
#include <functional>
#include <DxLib.h>
#include "Stage/StageManager.h"
#include "Order/Order.h"
#include "ActorBase.h"

class AnimationController;
class Collider;
class Capsule;
class Sphere;

class Player : public ActorBase
{

public:

	//�X�s�[�h
	static constexpr float SPEED_MOVE = 5.0f;	//�����X�s�[�h
	static constexpr float SPEED_RUN = 7.0f;	//����X�s�[�h

	//��]�����܂ł̎���
	static constexpr float TIME_ROT = 0.5f;

	//���G�t�F�N�g�����Ԋu
	static constexpr float TERM_FOOT_SMOKE = 0.3f;

	//���
	enum class STATE
	{
		NONE,
		PLAY,
		STOP,
	};

	//�A�j���[�V�������
	enum class ANIM_TYPE
	{
		IDLE,
		WALK,
		RUN,
		IDLE_HOLD,
		WALK_HOLD,
		JUMP,
	};

	//�R���X�g���N�^
	Player(void);

	//�f�X�g���N�^
	~Player(void);

	/// <summary>
	/// ����������
	/// </summary>
	void Init(void) override;

	/// <summary>
	/// �X�V����
	/// </summary>
	void Update(void) override;

	/// <summary>
	/// �`�揈��
	/// </summary>
	void Draw(void) override;

	/// <summary>
	/// �Փ˔���ɗp������R���C�_�̒ǉ�
	/// </summary>
	/// <param name="collider">�Փ˔��肷��R���C�_</param>
	void AddCollider(const std::weak_ptr<Collider> collider);

	/// <summary>
	/// �ǉ������R���C�_��S���폜����֐�
	/// </summary>
	void ClearCollider(void);

	/// <summary>
	/// �Փ˗p�J�v�Z���̎擾
	/// </summary>
	/// <returns>�v���C���[�ɒǏ]���Ă���J�v�Z��</returns>
	const Capsule& GetCapsule(void) const;

	/// <summary>
	/// �I�u�W�F�N�g����p�̋��̂̎擾
	/// </summary>
	/// <returns>�Փ˔���p�̋���</returns>
	const Sphere& GetSphere(void) const { return *sphere_; }

	/// <summary>
	/// �v���C���[��PLAY��Ԃ��ǂ���
	/// </summary>
	/// <param name=""></param>
	/// <returns>true:PLAY���@false:PLAY�ȊO</returns>
	const bool& IsPlay(void)const;

	/// <summary>
	/// �v���C���[���I�u�W�F�N�g�������Ă��邩�ǂ����ݒ肷��
	/// </summary>
	/// <param name="hold">true:�������@false:������</param>
	void SetIsHolding(const bool hold) { isHolding_ = hold; }

	/// <summary>
	/// �v���C���[�̏�����Ԃ��擾����
	/// </summary>
	/// <returns>true:�������@false:������</returns>
	const bool& GetIsHolding(void)const { return isHolding_; }

	/// <summary>
	/// �v���C���[�̏������Ă���A�C�e����ID���擾����
	/// </summary>
	/// <returns>�����Ă���A�C�e����ID</returns>
	const std::string& GetHoldItem(void)const { return holdItemId_; }

	/// <summary>
	/// �v���C���[�̏������Ă���A�C�e����ID��ݒ肷��
	/// </summary>
	/// <param name="item">��������A�C�e����ID</param>
	void SetHoldItem(const std::string item) { holdItemId_ = item; }

	/// <summary>
	/// �w�肳�ꂽSTATE�ɏ�Ԃ�ύX����
	/// </summary>
	/// <param name="state">�V�����ݒ肷��STATE�^�̏��</param>
	void ChangeState(const STATE state);

private:

	//�A�j���[�V����
	std::unique_ptr<AnimationController> animationController_;

	//��ԊǗ�
	STATE state_;

	//��ԊǗ�(��ԑJ�ڎ���������)
	std::map<STATE, std::function<void(void)>> stateChanges_;

	//��ԊǗ�(�X�V�X�e�b�v)
	std::function<void(void)> stateUpdate_;

	//�ړ��X�s�[�h
	float speed_;
	
	//�ړ�����
	VECTOR moveDir_;
	
	//�ړ���
	VECTOR movePow_;
	
	//�ړ���̍��W
	VECTOR movedPos_;

	//��]
	Quaternion playerRotY_;
	Quaternion goalQuaRot_;
	float stepRotTime_;
	
	//�Փ˔���ɗp������R���C�_
	std::vector<std::weak_ptr<Collider>> colliders_;
	
	//�Փ˃`�F�b�N
	VECTOR gravHitPosDown_;	//������(�J�v�Z���̉��̋���)
	VECTOR gravHitPosUp_;	//�����(�J�v�Z���̏�̋���)
	
	//�ۉe
	int imgShadow_;

	//�J�v�Z��
	std::unique_ptr<Capsule> capsule_;
	//����
	std::unique_ptr<Sphere> sphere_;

	//�����G�t�F�N�g
	int effectSmokeResId_;	//�G�t�F�N�g���\�[�XID
	int effectSmokePlayId_;	//�G�t�F�N�g�Đ�ID
	float stepFootSmoke_;	//�G�t�F�N�g�����܂ł̎��Ԍv���p

	//�t���[�����Ƃ̈ړ��l
	VECTOR moveDiff_;

	//�v���C���[���I�u�W�F�N�g�������Ă��邩�ǂ���
	bool isHolding_;

	//�����Ă���A�C�e����ID
	std::string holdItemId_;

	int chestFrmNo_;
	VECTOR chestPos_;

	/// <summary>
	/// �A�j���[�V��������������
	/// </summary>
	void InitAnimation(void);

	//��ԑJ��

	/// <summary>
	/// NONE��ԂɑJ�ڂ��鏈��
	/// </summary>
	void ChangeStateNone(void);
	/// <summary>	
	/// PLAY��ԂɑJ�ڂ��鏈��
	/// </summary>
	void ChangeStatePlay(void);
	/// <summary>
	/// STOP��ԂɑJ�ڂ��鏈��
	/// </summary>
	void ChangeStateStop(void);

	//�X�V�X�e�b�v

	/// <summary>
	/// NONE��Ԃ̍X�V����
	/// </summary>
	void UpdateNone(void);
	/// <summary>
	/// PLAY��Ԃ̍X�V����
	/// </summary>
	void UpdatePlay(void);
	/// <summary>
	/// STOP��Ԃ̍X�V����
	/// </summary>
	void UpdateStop(void);

	/// <summary>
	/// �e��`�悷�鏈��(�e�͉摜�őΉ�)
	/// </summary>
	void DrawShadow(void);

	//���� 

	/// <summary>
	/// �ړ�����
	/// </summary>
	void ProcessMove(void);

	/// <summary>
	/// ��]����(�ڕW�p�x��ݒ�)
	/// </summary>
	/// <param name="rotRad">�ڕW�p�x</param>
	void SetGoalRotate(const double rotRad);

	/// <summary>
	/// ��]���鏈��(���X�ɉ�])
	/// </summary>
	void Rotate(void);

	/// <summary>
	/// �Փ˔��菈��
	/// </summary>
	void Collision(void);
	
	/// <summary>
	/// �J�v�Z���̏Փ˔��菈��
	/// </summary>
	void CollisionCapsule(void);

	/// <summary>
	/// �����G�t�F�N�g��������
	/// </summary>
	void EffectFootSmoke(void);
};
