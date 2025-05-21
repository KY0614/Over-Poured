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

StageManager::StageManager(Vector2 mapSize, Player& player):player_(player)
{
	size_.height_ = mapSize.y;
	size_.width_ = mapSize.x;
}

StageManager::~StageManager(void)
{
}

void StageManager::Init(void)
{
	//モデル制御の基本情報
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
	transform_.MakeCollider(Collider::TYPE::STAGE);
	transform_.Update();

	//コーヒーマシン
	machine_ = std::make_unique<StageObject>("Coffee_Machine",75.0f,60.0f,60.0f);
	machine_->Init();
	machine_->SetPos(MACHINE_POS);

	//テーブル
	table_ = std::make_unique<StageObject>("Table", 60.0f, 60.0f, 60.0f);
	table_->Init();
	table_->SetPos(AsoUtility::VECTOR_ZERO);
	
	//ホット用カップ
	cupH_ = std::make_unique<StageObject>("Cup_Hot", 40.0f, 30.0f, 40.0f);
	cupH_->Init();
	cupH_->SetPos(CUPHOT_POS);
	
	//アイス用カップ
	cupI_ = std::make_unique<StageObject>("Cup_Ice", 40.0f, 30.0f, 40.0f);
	cupI_->Init();
	cupI_->SetPos(CUPICE_POS);
		
	//アイスディスペンサー(氷を入れるやつ）
	iceDispenser_ = std::make_unique<StageObject>("Ice_Dispenser", 60.0f, 20.0f, 60.0f);
	iceDispenser_->Init();
	iceDispenser_->SetPos(ICEDIS_POS);
		
	//蓋類
	libs_ = std::make_unique<StageObject>("Libs", 40.0f, 30.0f, 40.0f);
	libs_->Init();
	libs_->SetPos(LIBS_POS);
			
	//ゴミ箱
	dustBox_ = std::make_unique<StageObject>("Dust_Box", 40.0f, 70.0f, 40.0f);
	dustBox_->Init();
	dustBox_->SetPos(DUSTBOX_POS);

#ifdef _DEBUG

	//カウンター前の当たり判定用の球体
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

	//machine_->Update();
	//table_->Update();
	//cupH_->Update();
	//cupI_->Update();
	//iceDispenser_->Update();
	//libs_->Update();
	//dustBox_->Update();

	if (AsoUtility::IsHitSphereCube(VGet(0.0f, 60.0f, 50.0f), 20.0f, CUPHOT_POS, 40.0f, 30.0f, 40.0f))
	{

	}

#ifdef _DEBUG

	//ImGuiの操作を行う
	//UpdateDebugImGui();

#endif // _DEBUG

}

void StageManager::Draw(void)
{
	//モデルの描画

	//Zバッファを有効にする
	SetUseZBuffer3D(true);

	//Zバッファへの書き込みを有効にする
	SetWriteZBuffer3D(true);
	SetWriteZBufferFlag(true);

	MV1DrawModel(transform_.modelId);
	DrawSphere3D(sphereTran_.pos, 30, 8, 0xff0000, 0xff0000, false);
	machine_->Draw();
	table_->Draw();
	cupH_->Draw();
	cupI_->Draw();
	iceDispenser_->Draw();
	libs_->Draw();
	dustBox_->Draw();
}

bool StageManager::IsInBounds(int x, int y) const
{
	return (x >= 0 && x < size_.width_ && y >= 0 && y < size_.height_);
}

void StageManager::UpdateDebugImGui(void)
{
	//ウィンドウタイトル&開始処理
	ImGui::Begin("Player:Circle");
	//大きさ
	ImGui::Text("scale");
	ImGui::InputFloat("SclX", &sphereTran_.scl.x);
	ImGui::InputFloat("SclY", &sphereTran_.scl.y);
	ImGui::InputFloat("SclZ", &sphereTran_.scl.z);
	//角度
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

	//位置
	ImGui::Text("position");
	//構造体の先頭ポインタを渡し、xyzと連続したメモリ配置へアクセス
	ImGui::InputFloat3("Pos", &sphereTran_.pos.x);
	ImGui::SliderFloat("PosX", &sphereTran_.pos.x, 0.0f, 360.0f);
	ImGui::SliderFloat("PosY", &sphereTran_.pos.y, 0.0f, 360.0f);
	ImGui::SliderFloat("PosZ", &sphereTran_.pos.z, 0.0f, 1000.0f);

	//終了処理
	ImGui::End();
}
