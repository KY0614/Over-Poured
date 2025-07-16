#include <DxLib.h>
#include "Libs/ImGuiWrapper.h"
#include <EffekseerForDXLib.h>
#include "Manager/Generic/InputManager.h"
#include "Manager/Generic/ResourceManager.h"
#include "Manager/Generic/SceneManager.h"
#include "FpsControl.h"
#include "Application.h"

Application* Application::instance_ = nullptr;

const std::string Application::PATH_IMAGE = "Data/Image/";
const std::string Application::PATH_MODEL = "Data/Model/";
const std::string Application::PATH_EFFECT = "Data/Effect/";
const std::string Application::PATH_SHADER = "Data/Shader/";
const std::string Application::PATH_SOUND = "Data/Sound/";
const std::string Application::PATH_SCORE = "Data/Score/";

void Application::CreateInstance(void)
{
	if (instance_ == nullptr)
	{
		instance_ = new Application();
	}
	instance_->Init();
}

Application& Application::GetInstance(void)
{
	return *instance_;
}

void Application::Init(void)
{

	//�A�v���P�[�V�����̏����ݒ�
	SetWindowText(L"Over Poured");

	//�E�B���h�E�T�C�Y
	SetGraphMode(SCREEN_SIZE_X, SCREEN_SIZE_Y, 32);
	ChangeWindowMode(true);

	//DxLib�̏�����
	SetUseDirect3DVersion(DX_DIRECT3D_11);
	isInitFail_ = false;
	if (DxLib_Init() == -1)
	{
		isInitFail_ = true;
		return;
	}

	ImGuiWrapper::CreateInstance();

	//Effekseer�̏�����
	InitEffekseer();

	//�L�[���䏉����
	SetUseDirectInputFlag(true);
	InputManager::CreateInstance();

	//���\�[�X�Ǘ�������
	ResourceManager::CreateInstance();

	//�V�[���Ǘ�������
	SceneManager::CreateInstance();

	fps_ = std::make_unique<FpsControl>();
	fps_->Init();
}

void Application::Run(void)
{

	auto& inputManager = InputManager::GetInstance();
	auto& sceneManager = SceneManager::GetInstance();
	auto& imGuiWrapper = ImGuiWrapper::GetInstance();

	//�Q�[�����[�v
	while (ProcessMessage() == 0 && CheckHitKey(KEY_INPUT_ESCAPE) == 0)
	{

		//�t���[�����[�g�X�V
		//1/60�b�o�߂��Ă��Ȃ��Ȃ�ă��[�v
		if (!fps_->UpdateFrameRate())continue;

		inputManager.Update();
		imGuiWrapper.Update();
		sceneManager.Update();

		sceneManager.Draw();

		RenderVertex();
		imGuiWrapper.Draw();

		fps_->CalcFrameRate();	//�t���[�����[�g�v�Z

		ScreenFlip();

	}

}

void Application::Destroy(void)
{

	InputManager::GetInstance().Destroy();
	ResourceManager::GetInstance().Destroy();
	SceneManager::GetInstance().Destroy();
	ImGuiWrapper::GetInstance().Destroy();

	//Effekseer���I������B
	Effkseer_End();

	//DxLib�I��
	if (DxLib_End() == -1)
	{
		isReleaseFail_ = true;
	}

	delete instance_;

}

bool Application::IsInitFail(void) const
{
	return isInitFail_;
}

bool Application::IsReleaseFail(void) const
{
	return isReleaseFail_;
}

Application::Application(void)
{
	isInitFail_ = false;
	isReleaseFail_ = false;
	fps_ = nullptr;
}

void Application::InitEffekseer(void)
{
	if (Effekseer_Init(8000) == -1)
	{
		DxLib_End();
	}

	SetChangeScreenModeGraphicsSystemResetFlag(FALSE);

	Effekseer_SetGraphicsDeviceLostCallbackFunctions();
}
