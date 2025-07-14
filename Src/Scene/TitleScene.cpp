#include <string>
#include <DxLib.h>
#include "../Application.h"
#include "../Common/DebugDrawFormat.h"
#include "../Utility/AsoUtility.h"
#include "../Manager/Generic/SceneManager.h"
#include "../Manager/Generic/ResourceManager.h"
#include "../Manager/Generic/InputManager.h"
#include "../Manager/Generic/Camera.h"
#include "../Object/Common/AnimationController.h"
#include "../Object/SkyDome.h"
#include "TitleScene.h"

TitleScene::TitleScene(void)
{
	pushImg_ = -1;
	titleImg_ = -1;
	skyDome_ = nullptr;
	animationController_ = nullptr;
}

TitleScene::~TitleScene(void)
{

}

void TitleScene::Init(void)
{

	//�摜�ǂݍ���
	pushImg_ = ResourceManager::GetInstance().Load(ResourceManager::SRC::PUSH_SPACE).handleId_;
	titleImg_ = ResourceManager::GetInstance().Load(ResourceManager::SRC::TITLE_LOGO).handleId_;

	//�w�i
	spaceDomeTran_.pos = AsoUtility::VECTOR_ZERO;
	skyDome_ = std::make_unique<SkyDome>(spaceDomeTran_);
	skyDome_->Init();

	float size;

	//�L����
	charactor_.SetModel(ResourceManager::GetInstance().LoadModelDuplicate(ResourceManager::SRC::PLAYER));
	charactor_.pos = { -250.0f, -32.0f, -105.0f };
	size = 0.4f;
	charactor_.scl = { size, size, size };
	charactor_.quaRot = Quaternion::Euler(
		0.0f, AsoUtility::Deg2RadF(90.0f), 0.0f);
	charactor_.Update();

	//�A�j���[�V�����̐ݒ�
	std::string path = Application::PATH_MODEL + "Player/";
	animationController_ = std::make_unique<AnimationController>(charactor_.modelId);
	animationController_->Add(0, path + "Run.mv1", 20.0f);
	animationController_->Play(0);

	//��_�J����
	mainCamera->ChangeMode(Camera::MODE::FIXED_POINT);

}

void TitleScene::Update(void)
{

	//�V�[���J��
	InputManager& ins = InputManager::GetInstance();
	if (ins.IsInputTriggered("NextScene"))
	{
		SceneManager::GetInstance().ChangeScene(SceneManager::SCENE_ID::MOVIE);
	}

	//�L�����A�j���[�V����
	animationController_->Update();

	skyDome_->Update();

}

void TitleScene::Draw(void)
{
	//���S�������߂ɏk�����Ă���̂ŃW���M�[���ڗ����Ȃ��悤�Ƀo�C���j�A�@�ŕ`��
	SetDrawMode(DX_DRAWMODE_BILINEAR);

	skyDome_->Draw();

	MV1DrawModel(planet_.modelId);
	//MV1DrawModel(charactor_.modelId);

	//pushspace�̉摜
	DrawRotaGraph(
		Application::SCREEN_SIZE_X / 2, 500,
		1.0, 0.0, pushImg_, true);

	//���S�摜
	DrawRotaGraph(
		Application::SCREEN_SIZE_X / 2, Application::SCREEN_SIZE_Y/2 - LOGO_OFFSET_Y,
		0.5, 0.0, titleImg_, true);
	
}
