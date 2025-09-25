#include <chrono>
#include <DxLib.h>
#include <EffekseerForDXLib.h>
#include "../../Utility/CommonUtility.h"
#include "../../Common/Fader.h"
#include "../../Scene/TitleScene.h"
#include "../../Scene/PauseScene.h"
#include "../../Scene/TutorialScene.h"
#include "../../Scene/GameScene.h"
#include "../../Scene/ResultScene.h"
#include "Camera.h"
#include "../GameSystem/SoundManager.h"
#include "../../Object/UI/UIManager.h"
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
	SoundManager::CreateInstance();
	UIManager::CreateInstance();

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

	//���C�g�̌���
	lightDir_ = LIGHT_DIR;

	//3D�p�̐ݒ�
	Init3D();

	//�����V�[���̐ݒ�
	DoChangeScene(SCENE_ID::RESULT);

}

void SceneManager::Init3D(void)
{
	//SetBackgroundColor(0, 139, 139);
	//�w�i�F�ݒ�
	SetBackgroundColor(64, 64, 128);

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
	SetFogEnable(true);
	SetFogColor(5, 5, 5);
	SetFogStartEnd(10000.0f, 20000.0f);
}

void SceneManager::Update(void)
{
	ChangeLightTypeDir(lightDir_);
	if (scenes_.empty())
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
		//scene_->Update();
		scenes_.back()->Update();
	}

	//�J�����X�V
	camera_->Update();

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
	//scene_->Draw();
	for (auto& scene : scenes_) 
	{
		scene->Draw();
	}

	//��Ƀ|�X�g�G�t�F�N�g�p
	camera_->Draw();

	//Effekseer�ɂ��Đ����̃G�t�F�N�g��`�悷��B
	DrawEffekseer3D();
	
	//�Ó]�E���]
	fader_->Draw();
}

void SceneManager::Destroy(void)
{
	SoundManager::GetInstance().Destroy();
	UIManager::GetInstance().Destroy();
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

void SceneManager::ChangeScene(std::unique_ptr<SceneBase> _scene)
{
	if (scenes_.empty()) {
		//�󂾂�����V���������
		scenes_.push_back(std::move(_scene));
	}
	else {
		//�����̂��̂�V�������ɓ���ւ���
		scenes_.back() = std::move(_scene);
	}
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

void SceneManager::PushScene(std::unique_ptr<SceneBase> _scene)
{
	//�V�����ςނ̂ł��Ƃ��Ɠ����Ă���z�͂܂��폜����Ȃ�
	scenes_.push_back(std::move(_scene));
	scenes_.back()->Init();
}

void SceneManager::PopScene(void)
{
	//�ς�ł�����̂������āA���Ƃ��Ƃ��������̂𖖔��ɂ���
	if (scenes_.size() > 1) 
	{
		scenes_.pop_back();
	}
}

void SceneManager::JumpScene(std::unique_ptr<SceneBase> scene)
{
	scenes_.clear();
	scenes_.push_back(std::move(scene));
}

SceneManager::SceneManager(void)
{

	sceneId_ = SCENE_ID::NONE;
	waitSceneId_ = SCENE_ID::NONE;

	scene_ = nullptr;
	scenes_.clear();
	fader_ = nullptr;

	isSceneChanging_ = false;

	//�f���^�^�C��
	deltaTime_ = 1.0f / 60.0f;

	camera_ = nullptr;
	lightDir_ = CommonUtility::VECTOR_ZERO;
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
	SoundManager::GetInstance().Release();
	UIManager::GetInstance().Release();

	//�V�[����ύX����
	sceneId_ = sceneId;

	//���݂̃V�[�������
	if (scene_ != nullptr)
	{
		scene_.reset();
	}
	
	MakeScene(sceneId);

	scenes_.back()->Init();
	//scene_->Init();

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

void SceneManager::MakeScene(SCENE_ID sceneId)
{
	auto& resM = ResourceManager::GetInstance();
	std::unique_ptr<SceneBase> scene;
	switch (sceneId)
	{
	case SceneManager::SCENE_ID::NONE:
		break;
	
	case SceneManager::SCENE_ID::TITLE:
		scene = std::make_unique<TitleScene>();
		resM.InitTitle();
		break;

	case SceneManager::SCENE_ID::TUTORIAL:
		scene = std::make_unique<TutorialScene>();
		resM.InitTutorial();
		break;
	
	case SceneManager::SCENE_ID::GAME:
		scene = std::make_unique<GameScene>();
		resM.InitGame();
		break;
	
	case SceneManager::SCENE_ID::PAUSE:
		scene = std::make_unique<PauseScene>();
		break;

	case SceneManager::SCENE_ID::RESULT:
		scene = std::make_unique<ResultScene>();
		resM.InitResult();
		break;
	
	default:
		break;
	}

	if (scenes_.empty())
	{
		//�󂾂�����V���������
		scenes_.push_back(std::move(scene));
	}
	else
	{
		//�����̂��̂�V�������ɓ���ւ���
		scenes_.back() = std::move(scene);
	}
}