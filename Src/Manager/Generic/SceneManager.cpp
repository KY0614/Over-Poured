#include <chrono>
#include <DxLib.h>
#include <EffekseerForDXLib.h>
#include "../../Libs/ImGui/imgui.h"
#include "../../Common/Fader.h"
#include "../../Scene/TitleScene.h"
#include "../../Scene/MovieScene.h"
#include "../../Scene/SelectScene.h"
#include "../../Scene/SelectScene.h"
#include "../../Scene/TutorialScene.h"
#include "../../Scene/GameScene.h"
#include "../../Scene/ResultScene.h"
#include "Camera.h"
#include "ResourceManager.h"
#include "SceneManager.h"

SceneManager* SceneManager::instance_ = nullptr;

void SceneManager::CreateInstance()
{
	if (instance_ == nullptr)
	{
		instance_ = new SceneManager();
	}
	instance_->Init();
}

SceneManager& SceneManager::GetInstance(void)
{
	return *instance_;
}

void SceneManager::Init(void)
{

	sceneId_ = SCENE_ID::NONE;
	waitSceneId_ = SCENE_ID::NONE;

	fader_ = std::make_unique<Fader>();
	fader_->Init();

	//�J����
	camera_ = std::make_shared<Camera>();
	camera_->Init();

	isSceneChanging_ = false;

	//�f���^�^�C��
	preTime_ = std::chrono::system_clock::now();

	lightDir_ = { 0.0f, -0.5f, 0.3f };

	//3D�p�̐ݒ�
	Init3D();

	//�����V�[���̐ݒ�
	DoChangeScene(SCENE_ID::GAME);

}

void SceneManager::Init3D(void)
{
	//�w�i�F�ݒ�
	SetBackgroundColor(0, 139, 139);

	//Z�o�b�t�@��L���ɂ���
	SetUseZBuffer3D(true);

	//Z�o�b�t�@�ւ̏������݂�L���ɂ���
	SetWriteZBuffer3D(true);
	SetWriteZBufferFlag(true);

	//�o�b�N�J�����O��L���ɂ���
	SetUseBackCulling(true);

	//���C�g�̐ݒ�
	SetUseLighting(true);
	
	//���C�g�̐ݒ�
	//ChangeLightTypeDir({ 0.3f, -0.7f, 0.8f });
	ChangeLightTypeDir(lightDir_);

	//�t�H�O�ݒ�
	//SetFogEnable(true);
	//SetFogColor(5, 5, 5);
	//SetFogStartEnd(10000.0f, 20000.0f);

}

void SceneManager::Update(void)
{
	ChangeLightTypeDir(lightDir_);
	if (scene_ == nullptr)
	{
		return;
	}

	//�f���^�^�C��
	auto nowTime = std::chrono::system_clock::now();
	deltaTime_ = static_cast<float>(
		std::chrono::duration_cast<std::chrono::nanoseconds>(nowTime - preTime_).count() / 1000000000.0);
	preTime_ = nowTime;

	fader_->Update();
	if (isSceneChanging_)
	{
		Fade();
	}
	else
	{
		scene_->Update();
	}

	//�J�����X�V
	camera_->Update();

	UpdateDebugImGui();
}

void SceneManager::Draw(void)
{
	
	//�`���O���t�B�b�N�̈�̎w��
	//(�R�c�`��Ŏg�p����J�����̐ݒ�Ȃǂ����Z�b�g�����)
	SetDrawScreen(DX_SCREEN_BACK);

	//��ʂ�������
	ClearDrawScreen();

	//�J�����ݒ�
	camera_->SetBeforeDraw();

	//Effekseer�ɂ��Đ����̃G�t�F�N�g���X�V����B
	UpdateEffekseer3D();

	//�`��
	scene_->Draw();

	//��Ƀ|�X�g�G�t�F�N�g�p
	camera_->Draw();

	//Effekseer�ɂ��Đ����̃G�t�F�N�g��`�悷��B
	DrawEffekseer3D();
	
	//�Ó]�E���]
	fader_->Draw();

}

void SceneManager::Destroy(void)
{
	delete instance_;
}

void SceneManager::ChangeScene(SCENE_ID nextId)
{

	//�t�F�[�h�������I����Ă���V�[����ς���ꍇ�����邽�߁A
	//�J�ڐ�V�[���������o�ϐ��ɕێ�
	waitSceneId_ = nextId;

	//�t�F�[�h�A�E�g(�Ó])���J�n����
	fader_->SetFade(Fader::STATE::FADE_OUT);
	isSceneChanging_ = true;

}

SceneManager::SCENE_ID SceneManager::GetSceneID(void)
{
	return sceneId_;
}

float SceneManager::GetDeltaTime(void) const
{
	//return 1.0f / 60.0f;
	return deltaTime_;
}

std::weak_ptr<Camera> SceneManager::GetCamera(void) const
{
	return camera_;
}

SceneManager::SceneManager(void)
{

	sceneId_ = SCENE_ID::NONE;
	waitSceneId_ = SCENE_ID::NONE;

	scene_ = nullptr;
	fader_ = nullptr;

	isSceneChanging_ = false;

	//�f���^�^�C��
	deltaTime_ = 1.0f / 60.0f;

	camera_ = nullptr;

}

void SceneManager::ResetDeltaTime(void)
{
	deltaTime_ = 0.016f;
	preTime_ = std::chrono::system_clock::now();
}

void SceneManager::DoChangeScene(SCENE_ID sceneId)
{
	auto& resM = ResourceManager::GetInstance();
	//���\�[�X�̉��
	resM.Release();

	//�V�[����ύX����
	sceneId_ = sceneId;

	//���݂̃V�[�������
	if (scene_ != nullptr)
	{
		scene_.reset();
	}

	switch (sceneId_)
	{
	case SceneManager::SCENE_ID::NONE:
		break;
	case SceneManager::SCENE_ID::TITLE:
		scene_ = std::make_unique<TitleScene>();
		resM.InitTitle();
		break;
	case SceneManager::SCENE_ID::MOVIE:
		scene_ = std::make_unique<MovieScene>();
		break;
	case SceneManager::SCENE_ID::SELECT:
		scene_ = std::make_unique<SelectScene>();
		break;
	case SceneManager::SCENE_ID::TUTORIAL:
		scene_ = std::make_unique<TutorialScene>();
		break;
	case SceneManager::SCENE_ID::GAME:
		scene_ = std::make_unique<GameScene>();
		resM.InitGame();
		break;
	case SceneManager::SCENE_ID::RESULT:
		scene_ = std::make_unique<ResultScene>();
		break;
	default:
		break;
	}

	scene_->Init();

	ResetDeltaTime();

	waitSceneId_ = SCENE_ID::NONE;

}

void SceneManager::Fade(void)
{

	Fader::STATE fState = fader_->GetState();
	switch (fState)
	{
	case Fader::STATE::FADE_IN:
		//���]��
		if (fader_->IsEnd())
		{
			//���]���I��������A�t�F�[�h�����I��
			fader_->SetFade(Fader::STATE::NONE);
			isSceneChanging_ = false;
		}
		break;
	case Fader::STATE::FADE_OUT:
		//�Ó]��
		if (fader_->IsEnd())
		{
			//���S�ɈÓ]���Ă���V�[���J��
			DoChangeScene(waitSceneId_);
			//�Ó]���疾�]��
			fader_->SetFade(Fader::STATE::FADE_IN);
		}
		break;
	}

}

void SceneManager::UpdateDebugImGui(void)
{
	//�E�B���h�E�^�C�g��&�J�n����
	ImGui::Begin("SceneManager:Light");
	////�p�x
	//VECTOR rotDeg = VECTOR();
	//rotDeg.x = AsoUtility::Rad2DegF(angles_.x);
	//rotDeg.y = AsoUtility::Rad2DegF(angles_.y);
	//rotDeg.z = AsoUtility::Rad2DegF(angles_.z);
	//ImGui::Text("angle(deg)");
	//ImGui::SliderFloat("RotX", &rotDeg.x, -300.0f, 360.0f);
	//ImGui::SliderFloat("RotY", &rotDeg.y, -300.0f, 360.0f);
	//ImGui::SliderFloat("RotZ", &rotDeg.z, -300.0f, 360.0f);
	//angles_.x = AsoUtility::Deg2RadF(rotDeg.x);
	//angles_.y = AsoUtility::Deg2RadF(rotDeg.y);
	//angles_.z = AsoUtility::Deg2RadF(rotDeg.z);
	//�ʒu
	ImGui::Text("dir");
	//�\���̂̐擪�|�C���^��n���Axyz�ƘA�������������z�u�փA�N�Z�X
	ImGui::InputFloat3("Pos", &lightDir_.x);
	ImGui::SliderFloat("PosX", &lightDir_.x, -10.0f, 10.0f);
	ImGui::SliderFloat("PosY", &lightDir_.y, -10.0f, 10.0f);
	ImGui::SliderFloat("PosZ", &lightDir_.z, -10.0f, 10.0f);

	//�I������
	ImGui::End();
}
