#include <string>
#include <DxLib.h>
#include "../Application.h"
#include "../Libs/ImGui/imgui.h"
#include "../Utility/CommonUtility.h"
#include "../Manager/GameSystem/SoundManager.h"
#include "../Manager/Generic/SceneManager.h"
#include "../Manager/Generic/ResourceManager.h"
#include "../Manager/Generic/InputManager.h"
#include "../Manager/Generic/Camera.h"
#include "../Object/Common/AnimationController.h"
#include "../Renderer/ModelMaterial.h"
#include "../Renderer/ModelRenderer.h"
#include "TitleScene.h"

namespace
{
	//ハイライトの点滅間隔
	const float HIGH_LIGHT_INTERVAL = 1.2f;

	//UVスケール
	const float TILLING_SIZE = 35.0f;

	//環境光の強さ
	const float AMBIENT_COLOR = 0.2f;

	//画像の位置
	const int LOGO_OFFSET_Y = 80;	//ロゴのY位置
	const int LOGO_HEIGHT = 1024;	//ロゴの高さ
	const int PUSHIMG_OFFSET_Y = 100;	//押下画像のY位置
}

TitleScene::TitleScene(void)
{
	pushImg_ = -1;
	titleImg_ = -1;
	animationController_ = nullptr;
	isView_ = false;
	highlightTime_ = 0.0f;
}

TitleScene::~TitleScene(void)
{

}

void TitleScene::Init(void)
{
	//サウンド初期化
	InitSound();

	//画像読み込み
	LoadImages();

	//モデル初期化
	Init3DModel();

	const float animSpeed = 20.0f;
	//アニメーションの設定
	std::string path = Application::PATH_MODEL + "Player/";
	animationController_ = std::make_unique<AnimationController>(character_.modelId);
	animationController_->Add(0, path + "Idle.mv1", animSpeed);
	animationController_->Play(0);

	//定点カメラ
	mainCamera->ChangeMode(Camera::MODE::FIXED_POINT);
	isView_ = true;
}

void TitleScene::Update(void)
{
	//シーン遷移
	InputManager& ins = InputManager::GetInstance();
	//点滅時間更新
	highlightTime_ += SceneManager::GetInstance().GetDeltaTime();
	//スペースキーでチュートリアルへ
	if (ins.IsInputTriggered("Interact"))
	{
		//SE再生
		SoundManager::GetInstance().Play(SoundManager::SOUND::PUSH_SPACE);
		SceneManager::GetInstance().ChangeScene(SceneManager::SCENE_ID::TUTORIAL);
	}
	//点滅処理
	if (highlightTime_ >= HIGH_LIGHT_INTERVAL)
	{
		isView_ = !isView_;	//表示非表示切り替え
		highlightTime_ = 0.0f;
	}
	//キャラアニメーション
	animationController_->Update();
	//モデルの更新
	cafeTran_.Update();
	character_.Update();
	groundTran_.Update();
}

void TitleScene::Draw(void)
{
	//ロゴを小さめに縮小しているのでジャギーが目立たないようにバイリニア法で描画
	SetDrawMode(DX_DRAWMODE_BILINEAR);
	//床の描画(シェーダー使用して)
	renderer_->Draw();
	//モデルの描画
	MV1DrawModel(cafeTran_.modelId);
	MV1DrawModel(character_.modelId);

	//画面の大きさに合わせて拡大率を変える
	float screenScl =
		static_cast<float>(Application::SCREEN_SIZE_Y) /
		static_cast<float>(Application::SCREEN_MAX_SIZE_Y);
	//ロゴの拡大率
	const float logoScale = 0.8f;
	//ロゴは少し小さめに調整
	screenScl *= logoScale;

	//pushspace画像を配置するためにロゴの大きさを確保
	const int drawScl = (int)((float)(LOGO_HEIGHT / 2) * screenScl);
	if (isView_)
	{
		const float scale = 2.0f;
		//pushspaceの画像
		DrawRotaGraph(
			Application::SCREEN_SIZE_X / 2,
			Application::SCREEN_SIZE_Y / 2 + drawScl,
			scale, 0.0, pushImg_, true);
	}

	//ロゴ画像
	DrawRotaGraph(
		Application::SCREEN_SIZE_X / 2,
		Application::SCREEN_SIZE_Y/2 - LOGO_OFFSET_Y,
		screenScl, 0.0, titleImg_, true);
	
}

void TitleScene::Init3DModel(void)
{
	//お店の座標と回転角
	const VECTOR cafePos = { 250.0f,0.0f,300.0f };
	const float cafeRot = 90.0f;
	//お店
	cafeTran_.SetModel(
		ResourceManager::GetInstance().
		LoadModelDuplicate(ResourceManager::SRC::CAFE));
	cafeTran_.pos = cafePos;
	cafeTran_.scl = CommonUtility::VECTOR_ONE;
	cafeTran_.quaRot = Quaternion::Euler(
		0.0f, CommonUtility::Deg2RadF(cafeRot), 0.0f);
	cafeTran_.Update();
	//地面の座標と拡大率
	const VECTOR graoundPos = { -4500.0f, 0.0f, 6100.0f };
	const float cafeScl = 2.5f;
	//地面
	groundTran_.SetModel(
		ResourceManager::GetInstance().
		LoadModelDuplicate(ResourceManager::SRC::FLOOR));
	groundTran_.pos = graoundPos;
	groundTran_.scl = { cafeScl,cafeScl,cafeScl };
	groundTran_.quaRot = CommonUtility::VECTOR_ZERO;
	groundTran_.MakeCollider(Collider::TYPE::STAGE);
	groundTran_.Update();

	//マテリアル初期化
	InitMaterial();

	//キャラの座標
	const VECTOR charaPos = { -390.0f,0.0f, -265.0f };
	//キャラ
	character_.SetModel(
		ResourceManager::GetInstance().
		LoadModelDuplicate(ResourceManager::SRC::PLAYER));
	character_.pos = charaPos;
	character_.scl = CommonUtility::VECTOR_ONE;
	character_.quaRot = Quaternion();
	character_.Update();

}

void TitleScene::InitMaterial(void)
{
	//定数バッファ用の定数
	const int VSbufferSize = 1;
	const int PSbufferSize = 3;
	//モデル描画用
	material_ = std::make_unique<ModelMaterial>(
		"StdModelVS.cso", VSbufferSize,
		"StdModelPS.cso", PSbufferSize
	);
	//タイリングするのでテクスチャアドレスをWRAPに
	material_->SetTextureAddress(ModelMaterial::TEXADDRESS::WRAP);
	material_->SetTextureBuf(0, ResourceManager::GetInstance().
		Load(ResourceManager::SRC::GROUND).handleId_);

	//uvに渡すスケール値
	material_->AddConstBufVS({ TILLING_SIZE ,TILLING_SIZE,TILLING_SIZE,TILLING_SIZE });

	//色の影響度
	material_->AddConstBufPS({ CommonUtility::VECTOR_ONE.x,CommonUtility::VECTOR_ONE.y,CommonUtility::VECTOR_ONE.z,1.0f });

	//ライトの方向
	VECTOR light = GetLightDirection();
	//4つ目の値はダミー
	material_->AddConstBufPS({ light.x,light.y,light.z,0.0f });

	//環境光
	material_->AddConstBufPS({ AMBIENT_COLOR,AMBIENT_COLOR,AMBIENT_COLOR,AMBIENT_COLOR });

	//モデルレンダラー生成
	renderer_ = std::make_unique<ModelRenderer>(groundTran_.modelId, *material_);
}

void TitleScene::LoadImages(void)
{
	//画像読み込み
	//pushspace画像
	pushImg_ = ResourceManager::GetInstance().Load(
		ResourceManager::SRC::PUSH_SPACE).handleId_;
	//ロゴ画像
	titleImg_ = ResourceManager::GetInstance().Load(
		ResourceManager::SRC::TITLE_LOGO).handleId_;
}

void TitleScene::InitSound(void)
{
	auto& sound = SoundManager::GetInstance();

	//BGM登録して音量調節後に再生
	sound.Add(SoundManager::TYPE::BGM, SoundManager::SOUND::TITLE,
		ResourceManager::GetInstance().Load(ResourceManager::SRC::TITLE_BGM).handleId_);
	sound.AdjustVolume(SoundManager::SOUND::TITLE, VOLUME_MAX / 2);
	sound.Play(SoundManager::SOUND::TITLE);

	//SE登録して音量調整
	sound.Add(SoundManager::TYPE::SE, SoundManager::SOUND::PUSH_SPACE,
		ResourceManager::GetInstance().Load(ResourceManager::SRC::PUSH_SPACE_SE).handleId_);
	sound.AdjustVolume(SoundManager::SOUND::PUSH_SPACE, VOLUME_MAX / 3);

}