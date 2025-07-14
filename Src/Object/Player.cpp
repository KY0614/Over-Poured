#include <string>
#include <cassert>
#include<EffekseerForDXLib.h>
#include "../Application.h"
#include "../Utility/AsoUtility.h"
#include "../Libs/ImGui/imgui.h"
#include "../Common/DebugDrawFormat.h"
#include "../Manager/Generic/SceneManager.h"
#include "../Manager/Generic/ResourceManager.h"
#include "../Manager/Generic/InputManager.h"
#include "../Manager/Generic/Camera.h"
#include "Common/AnimationController.h"
#include "Common/Capsule.h"
#include "Common/Collider.h"
#include "Common/Sphere.h"
#include "Order/Order.h"
#include "Player.h"

Player::Player(void)
{

	animationController_ = nullptr;
	state_ = STATE::NONE;

	//��ԊǗ�
	stateChanges_.emplace(STATE::NONE, std::bind(&Player::ChangeStateNone, this));
	stateChanges_.emplace(STATE::PLAY, std::bind(&Player::ChangeStatePlay, this));
	stateChanges_.emplace(STATE::STOP, std::bind(&Player::ChangeStateStop, this));

	//�Փ˃`�F�b�N
	gravHitPosDown_ = AsoUtility::VECTOR_ZERO;
	gravHitPosUp_ = AsoUtility::VECTOR_ZERO;

	isHolding_ = false;
	holdItemId_ = "";
	chestFrmNo_ = 0;
}

Player::~Player(void)
{
}

void Player::Init(void)
{

	//���f���̊�{�ݒ�
	transform_.SetModel(ResourceManager::GetInstance().LoadModelDuplicate(
		ResourceManager::SRC::PLAYER));
	transform_.scl = {0.7f,0.7f,0.7f};
	transform_.pos = { -60.0f, 30.0f, -10.0f };
	transform_.quaRot = Quaternion();
	transform_.quaRotLocal =
		Quaternion::Euler({ 0.0f, AsoUtility::Deg2RadF(180.0f), 0.0f });
	transform_.Update();

	//�ۉe�摜
	imgShadow_ = ResourceManager::GetInstance().Load(
		ResourceManager::SRC::PLAYER_SHADOW).handleId_;

	chestFrmNo_ = MV1SearchFrame(transform_.modelId, L"mixamorig:Hips");
	chestPos_ = MV1GetFramePosition(transform_.modelId, chestFrmNo_);

	//�J�v�Z���R���C�_
	capsule_ = std::make_unique<Capsule>(transform_);
	capsule_->SetLocalPosTop({ 0.0f, 90.0f, 0.0f });
	capsule_->SetLocalPosDown({ 0.0f, -10.0f, 0.0f });
	capsule_->SetRadius(20.0f);
	
	//�I�u�W�F�N�g�p�R���C�_
	sphere_ = std::make_unique<Sphere>(transform_);
	sphere_->SetLocalPos({ 0.0f, chestPos_.y, 50.0f });
	sphere_->SetRadius(20.0f);

	//�����G�t�F�N�g
	effectSmokeResId_ = ResourceManager::GetInstance().Load(
		ResourceManager::SRC::FOOT_SMOKE).handleId_;	

	//�A�j���[�V�����̐ݒ�
	InitAnimation();

	//�������
	ChangeState(STATE::PLAY);

	stepFootSmoke_ = TERM_FOOT_SMOKE;

#ifdef _DEBUG

	//�J�E���^�[�O�̓����蔻��p�̋���
	sphereTran_.Update();

	sphereTran_.scl = AsoUtility::VECTOR_ONE;
	sphereTran_.pos = { 221.0f, 0.0f, 139.0f };
	sphereTran_.quaRot = Quaternion();
	sphereTran_.quaRotLocal =
		Quaternion::Euler({ 0.0f, AsoUtility::Deg2RadF(180.0f), 0.0f });

#endif // _DEBUG

	isHolding_ = false;
}

void Player::Update(void)
{
	chestPos_ = MV1GetFramePosition(transform_.modelId, chestFrmNo_);
	sphere_->SetLocalPos({ 0.0f, chestPos_.y, 50.0f });
	transform_.pos.y = 30.0f;

	//�X�V�X�e�b�v
	stateUpdate_();

	transform_.Update();
	sphereTran_.Update();

	//�A�j���[�V�����Đ�
	animationController_->Update();
}

void Player::Draw(void)
{

	//���f���̕`��
	MV1DrawModel(transform_.modelId);

	//�ۉe�`��
	DrawShadow();
}

void Player::AddCollider(std::weak_ptr<Collider> collider)
{
	colliders_.emplace_back(collider);
}

void Player::ClearCollider(void)
{
	colliders_.clear();
}

const Capsule& Player::GetCapsule(void) const
{
	return *capsule_;
}

bool Player::IsPlay(void)
{
	return state_ == STATE::PLAY;
}

void Player::SurveItem(void)
{
	isHolding_ = false;
	holdItemId_ = "";
}

void Player::InitAnimation(void)
{

	std::string path = Application::PATH_MODEL + "Player/";
	animationController_ = std::make_unique<AnimationController>(transform_.modelId);
	animationController_->Add((int)ANIM_TYPE::IDLE, path + "Idle.mv1", 30.0f);
	animationController_->Add((int)ANIM_TYPE::WALK, path + "Walk.mv1", 30.0f);
	animationController_->Add((int)ANIM_TYPE::RUN, path + "Walk.mv1", 30.0f);
	animationController_->Add((int)ANIM_TYPE::IDLE_HOLD, path + "Idle_Hold.mv1", 30.0f);
	animationController_->Add((int)ANIM_TYPE::WALK_HOLD, path + "Walk_Hold.mv1", 30.0f);

	animationController_->Play((int)ANIM_TYPE::IDLE);
}

void Player::ChangeState(STATE state)
{

	//��ԕύX
	state_ = state;

	//�e��ԑJ�ڂ̏�������
	stateChanges_[state_]();

}

void Player::ChangeStateNone(void)
{
	stateUpdate_ = std::bind(&Player::UpdateNone, this);
}

void Player::ChangeStatePlay(void)
{
	stateUpdate_ = std::bind(&Player::UpdatePlay, this);
}

void Player::ChangeStateStop(void)
{
	stateUpdate_ = std::bind(&Player::UpdateStop, this);
}

void Player::UpdateNone(void)
{
}

void Player::UpdatePlay(void)
{
	if (holdItemId_ == "")isHolding_ = false;
	else isHolding_ = true;

	//�ړ�����
	ProcessMove();

	//�ړ������ɉ�������]
	Rotate();

	//�Փ˔���
	Collision();

	//�����G�t�F�N�g
	EffectFootSmoke();

	//�d�͕����ɉ����ĉ�]������
	transform_.quaRot = Quaternion::Quaternion();
	transform_.quaRot = transform_.quaRot.Mult(playerRotY_);
}

void Player::UpdateStop(void)
{
	//�X�g�b�v�Ƃ������C���^���N�g���Ƃ�������
	//�C���^���N�g�p�̃A�j���[�V��������������
	animationController_->Play((int)ANIM_TYPE::IDLE_HOLD);
}

void Player::DrawShadow(void)
{
	int i, j;
	MV1_COLL_RESULT_POLY_DIM HitResDim;
	MV1_COLL_RESULT_POLY* HitRes;
	VERTEX3D Vertex[3];
	VECTOR SlideVec;
	int ModelHandle;

	//���C�e�B���O�𖳌��ɂ���
	SetUseLighting(FALSE);

	//�y�o�b�t�@��L���ɂ���
	SetUseZBuffer3D(TRUE);

	//�e�N�X�`���A�h���X���[�h�� CLAMP �ɂ���( �e�N�X�`���̒[����͒[�̃h�b�g�����X���� )
	SetTextureAddressMode(DX_TEXADDRESS_CLAMP);

	//�e�𗎂Ƃ����f���̐������J��Ԃ�
	for (auto c : colliders_)
	{
		//�`�F�b�N���郂�f���́Aj��0�̎��̓X�e�[�W���f���A1�ȏ�̏ꍇ�̓R���W�������f��

		ModelHandle = c.lock()->modelId_;

		float PLAYER_SHADOW_HEIGHT = 700.0f;
		float PLAYER_SHADOW_SIZE = 50.0f;

		//�v���C���[�̒����ɑ��݂���n�ʂ̃|���S�����擾
		HitResDim = MV1CollCheck_Capsule(ModelHandle, -1, transform_.pos, VAdd(transform_.pos, VGet(0.0f, -PLAYER_SHADOW_HEIGHT, 0.0f)), PLAYER_SHADOW_SIZE);

		//���_�f�[�^�ŕω��������������Z�b�g
		Vertex[0].dif = GetColorU8(255, 255, 255, 255);
		Vertex[0].spc = GetColorU8(0, 0, 0, 0);
		Vertex[0].su = 0.0f;
		Vertex[0].sv = 0.0f;
		Vertex[1] = Vertex[0];
		Vertex[2] = Vertex[0];

		//���̒����ɑ��݂���|���S���̐������J��Ԃ�
		HitRes = HitResDim.Dim;
		for (i = 0; i < HitResDim.HitNum; i++, HitRes++)
		{
			//�|���S���̍��W�͒n�ʃ|���S���̍��W
			Vertex[0].pos = HitRes->Position[0];
			Vertex[1].pos = HitRes->Position[1];
			Vertex[2].pos = HitRes->Position[2];

			//������Ǝ����グ�ďd�Ȃ�Ȃ��悤�ɂ���
			SlideVec = VScale(HitRes->Normal, 0.5f);
			Vertex[0].pos = VAdd(Vertex[0].pos, SlideVec);
			Vertex[1].pos = VAdd(Vertex[1].pos, SlideVec);
			Vertex[2].pos = VAdd(Vertex[2].pos, SlideVec);

			//�|���S���̕s�����x��ݒ肷��
			Vertex[0].dif.a = 0;
			Vertex[1].dif.a = 0;
			Vertex[2].dif.a = 0;
			if (HitRes->Position[0].y > transform_.pos.y - PLAYER_SHADOW_HEIGHT)
				Vertex[0].dif.a = 128 * (1.0f - fabs(HitRes->Position[0].y - transform_.pos.y) / PLAYER_SHADOW_HEIGHT);

			if (HitRes->Position[1].y > transform_.pos.y - PLAYER_SHADOW_HEIGHT)
				Vertex[1].dif.a = 128 * (1.0f - fabs(HitRes->Position[1].y - transform_.pos.y) / PLAYER_SHADOW_HEIGHT);

			if (HitRes->Position[2].y > transform_.pos.y - PLAYER_SHADOW_HEIGHT)
				Vertex[2].dif.a = 128 * (1.0f - fabs(HitRes->Position[2].y - transform_.pos.y) / PLAYER_SHADOW_HEIGHT);

			//�t�u�l�͒n�ʃ|���S���ƃv���C���[�̑��΍��W���犄��o��
			Vertex[0].u = (HitRes->Position[0].x - transform_.pos.x) / (PLAYER_SHADOW_SIZE * 2.0f) + 0.5f;
			Vertex[0].v = (HitRes->Position[0].z - transform_.pos.z) / (PLAYER_SHADOW_SIZE * 2.0f) + 0.5f;
			Vertex[1].u = (HitRes->Position[1].x - transform_.pos.x) / (PLAYER_SHADOW_SIZE * 2.0f) + 0.5f;
			Vertex[1].v = (HitRes->Position[1].z - transform_.pos.z) / (PLAYER_SHADOW_SIZE * 2.0f) + 0.5f;
			Vertex[2].u = (HitRes->Position[2].x - transform_.pos.x) / (PLAYER_SHADOW_SIZE * 2.0f) + 0.5f;
			Vertex[2].v = (HitRes->Position[2].z - transform_.pos.z) / (PLAYER_SHADOW_SIZE * 2.0f) + 0.5f;

			//�e�|���S����`��
			DrawPolygon3D(Vertex, 1, imgShadow_, TRUE);
		}

		//���o�����n�ʃ|���S�����̌�n��
		MV1CollResultPolyDimTerminate(HitResDim);
	}

	//���C�e�B���O��L���ɂ���
	SetUseLighting(TRUE);

	//�y�o�b�t�@�𖳌��ɂ���
	SetUseZBuffer3D(FALSE);
}

void Player::ProcessMove(void)
{
	InputManager& ins = InputManager::GetInstance();
	Quaternion cameraRot = mainCamera->GetQuaRotOutX();

	double rotRad = 0;

	//WASD�ňʒu��ς���
	VECTOR dir = AsoUtility::VECTOR_ZERO;
	movePow_ = AsoUtility::VECTOR_ZERO;
	//if (ins.IsInputPressed("Up")) { dir = cameraRot.GetForward();  rotRad = AsoUtility::Deg2RadF(0.0f); }
	//if (ins.IsInputPressed("Left")){ dir = cameraRot.GetLeft();  rotRad = AsoUtility::Deg2RadF(-90.0f); }
	//if (ins.IsInputPressed("Down")){ dir = cameraRot.GetBack();  rotRad = AsoUtility::Deg2RadF(180.0f); }
	//if (ins.IsInputPressed("Right")){dir = cameraRot.GetRight(); rotRad = AsoUtility::Deg2RadF(90.0f);	}

	if (ins.IsInputPressed("Up"))	dir.z += 1.0f;
	if (ins.IsInputPressed("Down")) dir.z -= 1.0f;
	if (ins.IsInputPressed("Right"))dir.x += 1.0f;
	if (ins.IsInputPressed("Left")) dir.x -= 1.0f;

	if (!AsoUtility::EqualsVZero(dir))
	{
		////�ړ��X�s�[�h
		//speed_ = SPEED_MOVE;
		//if (ins.IsInputPressed("Dash"))
		//{
		//	speed_ = SPEED_RUN;
		//}

		dir = VNorm(dir); //�����𐳋K��

		// �J������Y���p�x�����擾�iXZ���ʂ̉�]�����ŏ\���j
		float camYRad = mainCamera->GetQuaRot().y; // �������̓J������Y����]�p�i���W�A���j

		// ��]�s����g���ē��̓x�N�g�����񂷁iXZ���ʁj
		float sinY = sinf(camYRad);
		float cosY = cosf(camYRad);
		VECTOR worldDir = VGet(
			dir.x * cosY - dir.z * sinY,
			0.0f,
			dir.x * sinY + dir.z * cosY
		);

		// �ړ����x�̐ݒ�
		speed_ = ins.IsInputPressed("Dash") ? SPEED_RUN : SPEED_MOVE;
		moveDir_ = worldDir;
		movePow_ = VScale(worldDir, speed_);

		// �v���C���[�̌������ړ������ɍ��킹��
		double rotRad = atan2(worldDir.x, worldDir.z); // ���W�A��
		SetGoalRotate(rotRad);

		// �A�j���[�V�����i�W�����v���͐؂�ւ��Ȃ��j
		if (IsEndLanding())
		{
			if (speed_ == SPEED_RUN)
			{
				animationController_->Play((int)ANIM_TYPE::RUN);
			}
			else 
			{
				animationController_->Play((int)ANIM_TYPE::WALK);
				if (isHolding_)animationController_->Play((int)ANIM_TYPE::WALK_HOLD);
			}
		}

		////�ړ���
		//movePow_ = VScale(dir, speed_);

		////��]����
		//SetGoalRotate(rotRad);

		//if (IsEndLanding())
		//{
		//	//�A�j���[�V����
		//	if (ins.IsInputPressed("Dash"))
		//	{
		//		//��������A�j���[�V����
		//		animationController_->Play((int)ANIM_TYPE::FAST_RUN);
		//	}
		//	else
		//	{
		//		//����A�j���[�V����
		//		animationController_->Play((int)ANIM_TYPE::RUN);
		//	}
		//}
	}
	else
	{
		if (IsEndLanding() && !isHolding_)
		{
			animationController_->Play((int)ANIM_TYPE::IDLE);
		}
		else if (IsEndLanding() && isHolding_)
		{
			animationController_->Play((int)ANIM_TYPE::IDLE_HOLD);
		}
	}

}

void Player::SetGoalRotate(double rotRad)
{
	VECTOR cameraRot = mainCamera->GetAngles();
	Quaternion axis =
		Quaternion::AngleAxis(
			(double)cameraRot.y + rotRad, AsoUtility::AXIS_Y);
	
	//���ݐݒ肳��Ă����]�Ƃ̊p�x�������
	double angleDiff = Quaternion::Angle(axis, goalQuaRot_);
	
	//�������l
	if (angleDiff > 0.1)
	{
		stepRotTime_ = TIME_ROT;
	}
	goalQuaRot_ = axis;
}

void Player::Rotate(void)
{
	stepRotTime_ -= SceneManager::GetInstance().GetDeltaTime();
	
	//��]�̋��ʕ��
	playerRotY_ = Quaternion::Slerp(
		playerRotY_, goalQuaRot_, (TIME_ROT - stepRotTime_) / TIME_ROT);
}

void Player::Collision(void)
{
	//���ݍ��W���N�_�Ɉړ�����W�����߂�
	movedPos_ = VAdd(transform_.pos, movePow_);
	
	//�Փ�(�J�v�Z��)
	CollisionCapsule();
	
	//�ړ�
	moveDiff_ = VSub(movedPos_, transform_.pos);
	transform_.pos = movedPos_;
}

bool Player::IsEndLanding(void)
{
	bool ret = true;

	//�A�j���[�V�������W�����v�ł͂Ȃ�
	if (animationController_->GetPlayType() != (int)ANIM_TYPE::JUMP)
	{
		return ret;
	}
	
	//�A�j���[�V�������I�����Ă��邩
	if (animationController_->IsEnd())
	{
		return ret;
	}

	return false;
}

void Player::CollisionCapsule(void)
{
	//�J�v�Z�����ړ�������
	Transform trans = Transform(transform_);
	trans.pos = movedPos_;
	trans.Update();
	Capsule cap = Capsule(*capsule_, trans);
	//�J�v�Z���Ƃ̏Փ˔���
	for (const auto c : colliders_)
	{
		auto hits = MV1CollCheck_Capsule(
			c.lock()->modelId_, -1,
			cap.GetPosTop(), cap.GetPosDown(), cap.GetRadius());
		//�Փ˂��������̃|���S���ƏՓˉ������܂ŁA
		//�v���C���[�̈ʒu���ړ�������
		for (int i = 0; i < hits.HitNum; i++)
		{
			auto hit = hits.Dim[i];
			//�n�ʂƈقȂ�A�Փˉ���ʒu���s���Ȃ��߁A���x���ړ�������
			//���̎��A�ړ�����������́A�ړ��O���W�Ɍ����������ł�������A
			//�Փ˂����|���S���̖@�������������肷��
			for (int tryCnt = 0; tryCnt < 10; tryCnt++)
			{
				//�ēx�A���f���S�̂ƏՓˌ��o����ɂ́A���������߂���̂ŁA
				//�ŏ��̏Փ˔���Ō��o�����Փ˃|���S��1���ƏՓ˔�������
				int pHit = HitCheck_Capsule_Triangle(
					cap.GetPosTop(), cap.GetPosDown(), cap.GetRadius(),
					hit.Position[0], hit.Position[1], hit.Position[2]);

				if (pHit)
				{
					//�@���̕����ɂ�����Ƃ����ړ�������
					movedPos_ = VAdd(movedPos_, VScale(hit.Normal, 1.0f));
					movedPos_.y = 30.0f;
					//�J�v�Z�����ꏏ�Ɉړ�������
					trans.pos = movedPos_;
					trans.Update();
					continue;
				}
				break;
			}
		}
		//���o�����n�ʃ|���S�����̌�n��
		MV1CollResultPolyDimTerminate(hits);
	}
}

void Player::EffectFootSmoke(void)
{
	stepFootSmoke_ -= SceneManager::GetInstance().GetDeltaTime();

	float len = AsoUtility::MagnitudeF(moveDiff_);

	if (len >= 1.0f &&
		stepFootSmoke_ < 0.0f)
	{
		stepFootSmoke_ = TERM_FOOT_SMOKE;

		//�G�t�F�N�g�Đ�
		effectSmokePlayId_ = PlayEffekseer3DEffect(effectSmokeResId_);

		//�傫��
		float SCALE = 5.0f;
		SetScalePlayingEffekseer3DEffect(effectSmokePlayId_, SCALE, SCALE, SCALE);

		//�ʒu�̐ݒ�
		SetPosPlayingEffekseer3DEffect(
			effectSmokePlayId_,
			transform_.pos.x,
			transform_.pos.y,
			transform_.pos.z);
	}
}