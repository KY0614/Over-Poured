#include <string>
#include <DxLib.h>
#include "../Application.h"
#include "../Libs/ImGui/imgui.h"
#include "../Utility/AsoUtility.h"
#include "../Manager/GameSystem/SoundManager.h"
#include "../Manager/Generic/SceneManager.h"
#include "../Manager/Generic/ResourceManager.h"
#include "../Manager/Generic/InputManager.h"
#include "../Manager/Generic/Camera.h"
#include "../Object/Common/AnimationController.h"
#include "../Renderer/ModelMaterial.h"
#include "../Renderer/ModelRenderer.h"
#include "TitleScene.h"

TitleScene::TitleScene(void)
{
	pushImg_ = -1;
	titleImg_ = -1;
	animationController_ = nullptr;
}

TitleScene::~TitleScene(void)
{

}

void TitleScene::Init(void)
{
	auto& sound = SoundManager::GetInstance();

	//BGM登録して音量調節後に再生
	sound.Add(SoundManager::TYPE::BGM, SoundManager::SOUND::TITLE,
		ResourceManager::GetInstance().Load(ResourceManager::SRC::TITLE_BGM).handleId_);
	sound.AdjustVolume(SoundManager::SOUND::TITLE, 256 / 3);
	sound.Play(SoundManager::SOUND::TITLE);

	//SE登録して音量調整
	sound.Add(SoundManager::TYPE::SE, SoundManager::SOUND::PUSH_SPACE,
		ResourceManager::GetInstance().Load(ResourceManager::SRC::PUSH_SPACE_SE).handleId_);
	sound.AdjustVolume(SoundManager::SOUND::PUSH_SPACE, 256 / 2);

	//画像読み込み
	pushImg_ = ResourceManager::GetInstance().Load(ResourceManager::SRC::PUSH_SPACE).handleId_;
	titleImg_ = ResourceManager::GetInstance().Load(ResourceManager::SRC::TITLE_LOGO).handleId_;

	//お店
	cafeTran_.SetModel(
		ResourceManager::GetInstance().
		LoadModelDuplicate(ResourceManager::SRC::CAFE));
	cafeTran_.pos = { 250.0f, 0.0f, 300.0f };
	cafeTran_.scl = AsoUtility::VECTOR_ONE;
	cafeTran_.quaRot = Quaternion::Euler(
		0.0f, AsoUtility::Deg2RadF(90.0f), 0.0f);
	cafeTran_.Update();

	//地面
	graoundTran_.SetModel(
		ResourceManager::GetInstance().
		LoadModelDuplicate(ResourceManager::SRC::FLOOR));
	graoundTran_.pos = { -4500.0f, 0.0f, 6100.0f };
	graoundTran_.scl = { 2.5f,2.0f,2.2f };
	graoundTran_.quaRot = AsoUtility::VECTOR_ZERO;
	graoundTran_.MakeCollider(Collider::TYPE::STAGE);
	graoundTran_.Update();

	InitMaterial();

	//キャラ
	charactor_.SetModel(
		ResourceManager::GetInstance().
		LoadModelDuplicate(ResourceManager::SRC::PLAYER));
	charactor_.pos = { -390.0f,0.0f, -265.0f };
	charactor_.scl = AsoUtility::VECTOR_ONE;
	charactor_.quaRot = Quaternion::Euler(
		0.0f, AsoUtility::Deg2RadF(0.0f), 0.0f);
	charactor_.Update();

	//アニメーションの設定
	std::string path = Application::PATH_MODEL + "Player/";
	animationController_ = std::make_unique<AnimationController>(charactor_.modelId);
	animationController_->Add(0, path + "Idle.mv1", 20.0f);
	animationController_->Play(0);

	//定点カメラ
	mainCamera->ChangeMode(Camera::MODE::FIXED_POINT);

}

void TitleScene::Update(void)
{
	//シーン遷移
	InputManager& ins = InputManager::GetInstance();
	if (ins.IsInputTriggered("Interact"))
	{
		SoundManager::GetInstance().Play(SoundManager::SOUND::PUSH_SPACE);
		SceneManager::GetInstance().ChangeScene(SceneManager::SCENE_ID::GAME);
	}

	//キャラアニメーション
	animationController_->Update();

	cafeTran_.Update();
	charactor_.Update();
	graoundTran_.Update();
}

void TitleScene::Draw(void)
{
	//ロゴを小さめに縮小しているのでジャギーが目立たないようにバイリニア法で描画
	SetDrawMode(DX_DRAWMODE_BILINEAR);

	renderer_->Draw();

	//MV1DrawModel(graoundTran_.modelId);
	MV1DrawModel(cafeTran_.modelId);
	MV1DrawModel(charactor_.modelId);

	//画面の大きさに合わせて拡大率を変える
	float screenScl =
		static_cast<float>(Application::SCREEN_SIZE_Y) /
		static_cast<float>(Application::SCREEN_MAX_SIZE_Y);

	//ロゴは少し小さめに調整
	screenScl *= 0.8f;

	//pushspace画像を配置するためにロゴの大きさを確保
	int logoScl = (LOGO_HEIGHT / 2) * screenScl;
	//pushspaceの画像
	DrawRotaGraph(
		Application::SCREEN_SIZE_X / 2,
		Application::SCREEN_SIZE_Y / 2 + logoScl,
		2.0f, 0.0, pushImg_, true);

	//ロゴ画像
	DrawRotaGraph(
		Application::SCREEN_SIZE_X / 2,
		Application::SCREEN_SIZE_Y/2 - LOGO_OFFSET_Y,
		screenScl, 0.0, titleImg_, true);
	
}

void TitleScene::InitMaterial(void)
{
	//モデル描画用
	material_ = std::make_unique<ModelMaterial>(
		"StdModelVS.cso", 1,
		"StdModelPS.cso", 3
	);
	//タイリングするのでテクスチャアドレスをWRAPに
	material_->SetTextureAddress(ModelMaterial::TEXADDRESS::WRAP);
	material_->SetTextureBuf(0, ResourceManager::GetInstance().
		Load(ResourceManager::SRC::GROUND).handleId_);

	//uvに渡すスケール値
	material_->AddConstBufVS({ TILLING_SIZE ,TILLING_SIZE,TILLING_SIZE,TILLING_SIZE });

	//色の影響度
	material_->AddConstBufPS({ AsoUtility::VECTOR_ONE.x,AsoUtility::VECTOR_ONE.y,AsoUtility::VECTOR_ONE.z,1.0f });

	//ライトの方向
	VECTOR light = GetLightDirection();
	material_->AddConstBufPS({ light.x,light.y,light.z,1.0f });

	//環境光
	material_->AddConstBufPS({ AMBIENT_COLOR,AMBIENT_COLOR,AMBIENT_COLOR,AMBIENT_COLOR });

	renderer_ = std::make_unique<ModelRenderer>(graoundTran_.modelId, *material_);

}

void TitleScene::UpdateDebugImGui(void)
{
	//ウィンドウタイトル&開始処理
	ImGui::Begin("cafe");

	// 大きさ
	ImGui::Text("scale");
	ImGui::InputFloat("SclX", &graoundTran_.scl.x);
	ImGui::InputFloat("SclY", &graoundTran_.scl.y);
	ImGui::InputFloat("SclZ", &graoundTran_.scl.z);

	//位置
	ImGui::Text("position");
	//構造体の先頭ポインタを渡し、xyzと連続したメモリ配置へアクセス
	ImGui::InputFloat3("Pos", &cafeTran_.pos.x);
	ImGui::SliderFloat("PosX", &cafeTran_.pos.x, -10000.0f, 10000.0f);
	ImGui::SliderFloat("PosY", &cafeTran_.pos.y, -10000.0f, 10000.0f);
	ImGui::SliderFloat("PosZ", &cafeTran_.pos.z, -10000.0f, 10000.0f);

	//終了処理
	ImGui::End();
}