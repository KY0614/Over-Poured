#include <math.h>
#include <DxLib.h>
#include <EffekseerForDXLib.h>
#include "../Libs/ImGui/imgui.h"
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

	UpdateDebugImGui();
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
		//�J�����̏����ݒ�
		pos_ = FIXEDCAMERA_DEFAULT_POS;
		//�����_
		targetPos_ = FIXEDCAMERA_RELATIVE_POS;
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
	//VECTOR axisDeg = AsoUtility::VECTOR_ZERO;
	float rotPow = 1.0f * DX_PI_F / 180.0f;
	//��]����
	if (ins.IsNew(KEY_INPUT_UP)) { angles_.x += rotPow; }
	if (ins.IsNew(KEY_INPUT_DOWN)) { angles_.x -= rotPow; }
	if (ins.IsNew(KEY_INPUT_LEFT)) { angles_.y -= rotPow; }
	if (ins.IsNew(KEY_INPUT_RIGHT)) { angles_.y += rotPow; }

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

void Camera::SetBeforeDrawFixedPoint(void)
{
	//�Ȃɂ����Ȃ�
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
}

void Camera::UpdateDebugImGui(void)
{
	//�E�B���h�E�^�C�g��&�J�n����
	ImGui::Begin("Camera");
	//�p�x
	VECTOR rotDeg = VECTOR();
	rotDeg.x = AsoUtility::Rad2DegF(angles_.x);
	rotDeg.y = AsoUtility::Rad2DegF(angles_.y);
	rotDeg.z = AsoUtility::Rad2DegF(angles_.z);
	ImGui::Text("angle(deg)");
	ImGui::SliderFloat("RotX", &rotDeg.x, -300.0f, 360.0f);
	ImGui::SliderFloat("RotY", &rotDeg.y, -300.0f, 360.0f);
	ImGui::SliderFloat("RotZ", &rotDeg.z, -300.0f, 360.0f);
	angles_.x = AsoUtility::Deg2RadF(rotDeg.x);
	angles_.y = AsoUtility::Deg2RadF(rotDeg.y);
	angles_.z = AsoUtility::Deg2RadF(rotDeg.z);
	//�ʒu
	ImGui::Text("position");
	//�\���̂̐擪�|�C���^��n���Axyz�ƘA�������������z�u�փA�N�Z�X
	ImGui::InputFloat3("Pos", &pos_.x);
	ImGui::SliderFloat("PosX", &pos_.x, -500.0f, 1000.0f);
	ImGui::SliderFloat("PosY", &pos_.y, -500.0f, 1000.0f);
	ImGui::SliderFloat("PosZ", &pos_.z, -500.0f, 1000.0f);

	//�\���̂̐擪�|�C���^��n���Axyz�ƘA�������������z�u�փA�N�Z�X
	ImGui::InputFloat3("TargetPos", &targetPos_.x);
	ImGui::SliderFloat("TargetPosX", &targetPos_.x, -500.0f, 360.0f);
	ImGui::SliderFloat("TargetPosY", &targetPos_.y, -500.0f, 360.0f);
	ImGui::SliderFloat("TargetPosZ", &targetPos_.z, -500.0f, 1000.0f);
	//�I������
	ImGui::End();
}
