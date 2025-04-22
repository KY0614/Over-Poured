#pragma once
#include <memory>
#include <vector>
#include <map>
#include <functional>
#include <DxLib.h>
#include "Stage.h"
#include "ActorBase.h"

class AnimationController;
class Collider;
class Capsule;

class Player : public ActorBase
{

public:

	//�X�s�[�h
	static constexpr float SPEED_MOVE = 5.0f;
	static constexpr float SPEED_RUN = 10.0f;

	//��]�����܂ł̎���
	static constexpr float TIME_ROT = 1.0f;

	//���G�t�F�N�g�����Ԋu
	static constexpr float TERM_FOOT_SMOKE = 0.3f;

	//���
	enum class STATE
	{
		NONE,
		PLAY,
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

	bool IsPlay(void);

	VECTOR GetHitPos(void);
	VECTOR GetHitNormal(void);

	void UpdateDebugImGui(void);

private:
	Transform spereTran_;

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

	//�����G�t�F�N�g
	int effectSmokeResId_;
	int effectSmokePlayId_;
	float stepFootSmoke_;	

	//�t���[�����Ƃ̈ړ��l
	VECTOR moveDiff_;
	
	//�X�Ίp
	float slopeAngleDeg_;
	//�X�΂̗�
	VECTOR slopePow_;
	//�X�΂̕���
	VECTOR slopeDir_;
	//�Փ˂��Ă���n�ʃ|���S���̖@��
	VECTOR hitNormal_;
	//�Փ˂��Ă���n�ʂƂ̍��W
	VECTOR hitPos_;
	
	void InitAnimation(void);

	//��ԑJ��
	void ChangeState(STATE state);
	void ChangeStateNone(void);
	void ChangeStatePlay(void);

	//�X�V�X�e�b�v
	void UpdateNone(void);
	void UpdatePlay(void);

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
