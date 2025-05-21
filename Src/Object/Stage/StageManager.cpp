#include <DxLib.h>
#include <fstream>
#include"../Libs/nlohmann/json.hpp"
#include "../Utility/AsoUtility.h"
#include "../Libs/ImGui/imgui.h"
#include "../Manager/Generic/ResourceManager.h"
#include "../Player.h"
#include "../Common/Transform.h"
#include "StageObjectLibrary.h"
#include "StageObject.h"
#include "StageManager.h"

StageManager::StageManager(Vector2 mapSize)
{
	size_.height_ = mapSize.y;
	size_.width_ = mapSize.x;
}

StageManager::~StageManager(void)
{
}

void StageManager::Init(void)
{
	//���f������̊�{���
	transform_.SetModel(
		ResourceManager::GetInstance().LoadModelDuplicate(
			ResourceManager::SRC::STAGE));
	transform_.scl = AsoUtility::VECTOR_ONE;
	transform_.pos = AsoUtility::VECTOR_ZERO;
	transform_.quaRot = Quaternion::Euler(
		0.0f,
		AsoUtility::Deg2RadF(0.0f),
		0.0f
	);
	transform_.quaRotLocal = Quaternion();
	transform_.Update();

	machine_ = std::make_unique<StageObject>("Coffee_Machine");
	machine_->Init();
	machine_->SetPos(AsoUtility::VECTOR_ZERO);

#ifdef _DEBUG

	//�J�E���^�[�O�̓����蔻��p�̋���
	sphereTran_.Update();

	sphereTran_.scl = AsoUtility::VECTOR_ONE;
	sphereTran_.pos = { 221.0f, 0.0f, 271.0f };
	sphereTran_.quaRot = Quaternion();
	sphereTran_.quaRotLocal =
		Quaternion::Euler({ 0.0f, AsoUtility::Deg2RadF(180.0f), 0.0f });

#endif // _DEBUG
}

void StageManager::Update(void)
{
	transform_.Update();
	sphereTran_.Update();

	machine_->Update();

#ifdef _DEBUG

	//ImGui�̑�����s��
	//UpdateDebugImGui();

#endif // _DEBUG

}

void StageManager::Draw(void)
{
	//���f���̕`��
	machine_->Draw();
	MV1DrawModel(transform_.modelId);
	DrawSphere3D(sphereTran_.pos, 30, 8, 0xff0000, 0xff0000, false);

	//		if (grid_[y][x]) {
}

bool StageManager::IsInBounds(int x, int y) const
{
	return (x >= 0 && x < size_.width_ && y >= 0 && y < size_.height_);
}

void StageManager::UpdateDebugImGui(void)
{
	//�E�B���h�E�^�C�g��&�J�n����
	ImGui::Begin("Player:Circle");
	//�傫��
	ImGui::Text("scale");
	ImGui::InputFloat("SclX", &sphereTran_.scl.x);
	ImGui::InputFloat("SclY", &sphereTran_.scl.y);
	ImGui::InputFloat("SclZ", &sphereTran_.scl.z);
	//�p�x
	VECTOR rotDeg = VECTOR();
	rotDeg.x = AsoUtility::Rad2DegF(sphereTran_.rot.x);
	rotDeg.y = AsoUtility::Rad2DegF(sphereTran_.rot.y);
	rotDeg.z = AsoUtility::Rad2DegF(sphereTran_.rot.z);
	ImGui::Text("angle(deg)");
	ImGui::SliderFloat("RotX", &rotDeg.x, 0.0f, 360.0f);
	ImGui::SliderFloat("RotY", &rotDeg.y, 0.0f, 360.0f);
	ImGui::SliderFloat("RotZ", &rotDeg.z, 0.0f, 360.0f);
	sphereTran_.rot.x = AsoUtility::Deg2RadF(rotDeg.x);
	sphereTran_.rot.y = AsoUtility::Deg2RadF(rotDeg.y);
	sphereTran_.rot.z = AsoUtility::Deg2RadF(rotDeg.z);
	//�ʒu
	ImGui::Text("position");
	//�\���̂̐擪�|�C���^��n���Axyz�ƘA�������������z�u�փA�N�Z�X
	ImGui::InputFloat3("Pos", &sphereTran_.pos.x);
	ImGui::SliderFloat("PosX", &sphereTran_.pos.x, 0.0f, 360.0f);
	ImGui::SliderFloat("PosY", &sphereTran_.pos.y, 0.0f, 360.0f);
	ImGui::SliderFloat("PosZ", &sphereTran_.pos.z, 0.0f, 1000.0f);
	//�I������
	ImGui::End();
}
