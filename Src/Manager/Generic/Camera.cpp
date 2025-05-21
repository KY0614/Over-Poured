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

	//クリップ距離を設定する(SetDrawScreenでリセットされる)
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

	//カメラの設定(位置と注視点による制御)
	SetCameraPositionAndTargetAndUpVec(
		pos_, 
		targetPos_, 
		cameraUp_
	);

	//DXライブラリのカメラとEffekseerのカメラを同期する。
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

	//カメラの初期設定
	SetDefault();

	//カメラモードの変更
	mode_ = mode;

	//変更時の初期化処理
	switch (mode_)
	{
	case Camera::MODE::FIXED_POINT:
		//カメラの初期設定
		pos_ = FIXEDCAMERA_DEFAULT_POS;
		//注視点
		targetPos_ = FIXEDCAMERA_RELATIVE_POS;
		break;
	case Camera::MODE::FOLLOW:
		break;
	}
}

void Camera::SetDefault(void)
{

	//カメラの初期設定
	pos_ = DEFAULT_CAMERA_POS;

	//注視点
	targetPos_ = AsoUtility::VECTOR_ZERO;

	//カメラの上方向
	cameraUp_ = AsoUtility::DIR_U;

	angles_.x = AsoUtility::Deg2RadF(30.0f);
	angles_.y = 0.0f;
	angles_.z = 0.0f;

	rot_ = Quaternion();

}

void Camera::SyncFollow(void)
{
	//追従先の位置
	VECTOR pos = followTransform_->pos;

	//追従先の向き
	Quaternion followRot = Quaternion::Quaternion();

	//注視点(通常重力でいうところのY値を追従対象と同じにする)
	VECTOR localPos = rotOutX_.PosAxis(LOCAL_F2T_POS);
	targetPos_ = VAdd(pos, localPos);

	//カメラ位置
	localPos = rot_.PosAxis(LOCAL_F2C_POS);
	pos_ = VAdd(pos, localPos);

	//正面から設定されたY軸分、回転させる
	rotOutX_ = followRot.Mult(Quaternion::AngleAxis(angles_.y, AsoUtility::AXIS_Y));

	//正面から設定されたX軸分、回転させる
	rot_ = rotOutX_.Mult(Quaternion::AngleAxis(angles_.x, AsoUtility::AXIS_X));

	rot_ = Quaternion::Slerp(rot_, rot_, 0.1f);

	//カメラの上方向
	cameraUp_ = followRot.GetUp();
}

void Camera::ProcessRot(void)
{
	InputManager& ins = InputManager::GetInstance();

	//回転軸と量を決める
	//VECTOR axisDeg = AsoUtility::VECTOR_ZERO;
	float rotPow = 1.0f * DX_PI_F / 180.0f;
	//回転処理
	if (ins.IsNew(KEY_INPUT_UP)) { angles_.x += rotPow; }
	if (ins.IsNew(KEY_INPUT_DOWN)) { angles_.x -= rotPow; }
	if (ins.IsNew(KEY_INPUT_LEFT)) { angles_.y -= rotPow; }
	if (ins.IsNew(KEY_INPUT_RIGHT)) { angles_.y += rotPow; }

	//x軸回転の制限（上は４０度、下は１５度）
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
	//なにもしない
}

void Camera::SetBeforeDrawFollow(void)
{

	//カメラ操作
	ProcessRot();

	//追従対象との相対位置を同期
	SyncFollow();

}

void Camera::SetBeforeDrawFree(void)
{
	//カメラ操作
	ProcessRot();
}

void Camera::UpdateDebugImGui(void)
{
	//ウィンドウタイトル&開始処理
	ImGui::Begin("Camera");
	//角度
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
	//位置
	ImGui::Text("position");
	//構造体の先頭ポインタを渡し、xyzと連続したメモリ配置へアクセス
	ImGui::InputFloat3("Pos", &pos_.x);
	ImGui::SliderFloat("PosX", &pos_.x, -500.0f, 1000.0f);
	ImGui::SliderFloat("PosY", &pos_.y, -500.0f, 1000.0f);
	ImGui::SliderFloat("PosZ", &pos_.z, -500.0f, 1000.0f);

	//構造体の先頭ポインタを渡し、xyzと連続したメモリ配置へアクセス
	ImGui::InputFloat3("TargetPos", &targetPos_.x);
	ImGui::SliderFloat("TargetPosX", &targetPos_.x, -500.0f, 360.0f);
	ImGui::SliderFloat("TargetPosY", &targetPos_.y, -500.0f, 360.0f);
	ImGui::SliderFloat("TargetPosZ", &targetPos_.z, -500.0f, 1000.0f);
	//終了処理
	ImGui::End();
}
