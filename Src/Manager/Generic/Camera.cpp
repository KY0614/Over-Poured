#include <math.h>
#include <DxLib.h>
#include <EffekseerForDXLib.h>
#include "../../Libs/ImGui/imgui.h"
#include "../../Utility/AsoUtility.h"
#include "../Generic/InputManager.h"
#include "../../Object/Common/Transform.h"
#include "Camera.h"

Camera::Camera(void)
{
	angles_ = VECTOR();
	cameraUp_ = VECTOR();
	mode_ = MODE::NONE;
	pos_ = AsoUtility::VECTOR_ZERO;
	targetPos_ = AsoUtility::VECTOR_ZERO;
	followTransform_ = nullptr;
}

Camera::~Camera(void)
{
}

void Camera::Init(void)
{

	ChangeMode(MODE::FIXED_POINT);
}

void Camera::Update(void)
{
}

void Camera::SetBeforeDraw(void)
{

	//�N���b�v������ݒ肷��(SetDrawScreen�Ń��Z�b�g�����)
	SetCameraNearFar(CAMERA_NEAR, CAMERA_FAR);

	switch (mode_)
	{
	case Camera::MODE::FIXED_POINT:
		SetBeforeDrawFixedPoint();
		break;

	case Camera::MODE::TOP_FIXED:
		SetBeforeDrawTopFixed();
		break;

	case Camera::MODE::FOLLOW:
		SetBeforeDrawFollow();
		break;

	case Camera::MODE::FREE:
		SetBeforeDrawFree();
		break;

	default:
		break;
	}
	//�J�����̐ݒ�(�ʒu�ƒ����_�ɂ�鐧��)
	SetCameraPositionAndTargetAndUpVec(
		pos_, 
		targetPos_, 
		cameraUp_
	);

	//DX���C�u�����̃J������Effekseer�̃J�����𓯊�����B
	Effekseer_Sync3DSetting();

	//UpdateDebugImGui();
}

void Camera::Draw(void)
{
}

void Camera::SetFollow(const Transform* follow)
{
	followTransform_ = follow;
}

VECTOR Camera::GetPos(void) const
{
	return pos_;
}

VECTOR Camera::GetAngles(void) const
{
	return angles_;
}

VECTOR Camera::GetTargetPos(void) const
{
	return targetPos_;
}

Quaternion Camera::GetQuaRot(void) const
{
	return rot_;
}

Quaternion Camera::GetQuaRotOutX(void) const
{
	return rotOutX_;
}

VECTOR Camera::GetForward(void) const
{
	return VNorm(VSub(targetPos_, pos_));
}

void Camera::ChangeMode(MODE mode)
{

	//�J�����̏����ݒ�
	SetDefault();

	//�J�������[�h�̕ύX
	mode_ = mode;

	//�ύX���̏���������
	switch (mode_)
	{
	case Camera::MODE::FIXED_POINT:
		break;
	case Camera::MODE::TOP_FIXED:
		//�J�����̏����ݒ�
		pos_ = FIXEDTOP_CAMERA_POS;
		//�����_
		targetPos_ = FIXEDTOP_CAMERA_RELATIVE_POS;
		break;	
	case Camera::MODE::FOLLOW:
		break;
	}
}

void Camera::SetDefault(void)
{

	//�J�����̏����ݒ�
	pos_ = DEFAULT_CAMERA_POS;

	//�����_
	targetPos_ = AsoUtility::VECTOR_ZERO;

	//�J�����̏����
	cameraUp_ = AsoUtility::DIR_U;

	angles_.x = AsoUtility::Deg2RadF(30.0f);
	angles_.y = 0.0f;
	angles_.z = 0.0f;

	rot_ = Quaternion();

}

void Camera::SyncFollow(void)
{
	//�Ǐ]��̈ʒu
	VECTOR pos = followTransform_->pos;

	//�Ǐ]��̌���
	Quaternion followRot = Quaternion::Quaternion();

	//�����_(�ʏ�d�͂ł����Ƃ����Y�l��Ǐ]�ΏۂƓ����ɂ���)
	VECTOR localPos = rotOutX_.PosAxis(LOCAL_F2T_POS);
	targetPos_ = VAdd(pos, localPos);

	//�J�����ʒu
	localPos = rot_.PosAxis(LOCAL_F2C_POS);
	pos_ = VAdd(pos, localPos);

	//���ʂ���ݒ肳�ꂽY�����A��]������
	rotOutX_ = followRot.Mult(Quaternion::AngleAxis(angles_.y, AsoUtility::AXIS_Y));

	//���ʂ���ݒ肳�ꂽX�����A��]������
	rot_ = rotOutX_.Mult(Quaternion::AngleAxis(angles_.x, AsoUtility::AXIS_X));

	rot_ = Quaternion::Slerp(rot_, rot_, 0.1f);

	//�J�����̏����
	cameraUp_ = followRot.GetUp();
}

void Camera::ProcessRot(void)
{
	InputManager& ins = InputManager::GetInstance();

	//��]���Ɨʂ����߂�
	float rotPow = 1.0f * DX_PI_F / 180.0f;
	//��]����
	if (ins.IsInputPressed("CameraUp")) { angles_.x += rotPow; }
	if (ins.IsInputPressed("CameraDown")) { angles_.x -= rotPow; }
	if (ins.IsInputPressed("CameraLeft")) { angles_.y -= rotPow; }
	if (ins.IsInputPressed("CameraRight")) { angles_.y += rotPow; }

	//x����]�̐����i��͂S�O�x�A���͂P�T�x�j
	if (angles_.x > LIMIT_X_UP_RAD)
	{
		angles_.x = LIMIT_X_UP_RAD;
	}
	else if (angles_.x < -LIMIT_X_DW_RAD)
	{
		angles_.x = -LIMIT_X_DW_RAD;
	}
}

void Camera::ProcessMove(void)
{
	InputManager& ins = InputManager::GetInstance();

	VECTOR dir = AsoUtility::VECTOR_ZERO;
	if (ins.IsInputPressed("CameraFront"))	pos_.z += 5.0f; targetPos_.z += 5.0f;
	if (ins.IsInputPressed("CameraBack"))	pos_.z -= 5.0f; targetPos_.z -= 5.0f;
	if (ins.IsInputPressed("CameraR"))		pos_.x += 5.0f;	targetPos_.x += 5.0f;
	if (ins.IsInputPressed("CameraL"))		pos_.x -= 5.0f;	targetPos_.x -= 5.0f;

	if (ins.IsInputPressed("CameraRise"))	pos_.y += 5.0f;	targetPos_.y += 5.0f;
	if (ins.IsInputPressed("CameraDescent"))pos_.y -= 5.0f;	targetPos_.y -= 5.0f;
}

void Camera::SetBeforeDrawFixedPoint(void)
{
	//�Ȃɂ����Ȃ�
}

void Camera::SetBeforeDrawTopFixed(void)
{
}

void Camera::SetBeforeDrawFollow(void)
{
	//�J��������
	ProcessRot();

	//�Ǐ]�ΏۂƂ̑��Έʒu�𓯊�
	SyncFollow();

}

void Camera::SetBeforeDrawFree(void)
{
	//�J��������
	ProcessRot();

	ProcessMove();	
}

void Camera::UpdateDebugImGui(void)
{
	//�E�B���h�E�^�C�g��&�J�n����
	ImGui::Begin("Camera");

	//�ʒu
	ImGui::Text("position");
	//�\���̂̐擪�|�C���^��n���Axyz�ƘA�������������z�u�փA�N�Z�X
	ImGui::InputFloat3("Pos", &pos_.x);
	ImGui::SliderFloat("PosX", &pos_.x, -800.0f, 1000.0f);
	ImGui::SliderFloat("PosY", &pos_.y, -800.0f, 1000.0f);
	ImGui::SliderFloat("PosZ", &pos_.z, -800.0f, 1000.0f);

	//�ʒu
	ImGui::Text("target");
	//�\���̂̐擪�|�C���^��n���Axyz�ƘA�������������z�u�փA�N�Z�X
	ImGui::InputFloat3("target", &targetPos_.x);
	ImGui::SliderFloat("targetX", &targetPos_.x, -800.0f, 1000.0f);
	ImGui::SliderFloat("targetY", &targetPos_.y, -800.0f, 1000.0f);
	ImGui::SliderFloat("targetZ", &targetPos_.z, -800.0f, 1000.0f);

	//�I������
	ImGui::End();
}