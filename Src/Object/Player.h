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
	static constexpr float SPEED_MOVE = 5.0f;
	static constexpr float SPEED_RUN = 7.0f;

	//��]�����܂ł̎���
	static constexpr float TIME_ROT = 1.0f;

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
		RUN,
		FAST_RUN,
		JUMP,
		FALLING
	};

	//�R���X�g���N�^
	Player(void);

	//�f�X�g���N�^
	~Player(void);

	void Init(void) override;
	void Update(void) override;
	void Draw(void) override;

	//�Փ˔���ɗp������R���C�_����
	void AddCollider(std::weak_ptr<Collider> collider);

	void ClearCollider(void);

	//�Փ˗p�J�v�Z���̎擾
	const Capsule& GetCapsule(void) const;

	const Sphere& GetSphere(void) const { return *sphere_; }

	bool IsPlay(void);

	void SetIsHoldiong(bool hold) { isHolding_ = hold; }

	void SurveItem(void);

	bool GetIsHolding(void)const { return isHolding_; }
	std::string GetHoldItem(void) { return holdItemId_; }

	void SetHoldItem(std::string item) { holdItemId_ = item; }

	void ChangeState(STATE state);

	void UpdateDebugImGui(void);

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
	VECTOR gravHitPosDown_;
	VECTOR gravHitPosUp_;
	
	//�ۉe
	int imgShadow_;

	//�J�v�Z��
	std::unique_ptr<Capsule> capsule_;
	std::unique_ptr<Sphere> sphere_;

	//�����G�t�F�N�g
	int effectSmokeResId_;
	int effectSmokePlayId_;
	float stepFootSmoke_;	

	//�t���[�����Ƃ̈ړ��l
	VECTOR moveDiff_;

	bool isHolding_;
	std::string holdItemId_;

	void InitAnimation(void);

	//��ԑJ��
	void ChangeStateNone(void);
	void ChangeStatePlay(void);
	void ChangeStateStop(void);

	//�X�V�X�e�b�v
	void UpdateNone(void);
	void UpdatePlay(void);
	void UpdateStop(void);

	//�`��n
	void DrawDebug(void);
	void DrawShadow(void);

	//���� 
	void ProcessMove(void);

	//��]
	void SetGoalRotate(double rotRad);
	void Rotate(void);

	//�Փ˔���
	void Collision(void);
	
	//���n���[�V�����I��
	bool IsEndLanding(void);

	void CollisionCapsule(void);

	//�����G�t�F�N�g
	void EffectFootSmoke(void);
};
