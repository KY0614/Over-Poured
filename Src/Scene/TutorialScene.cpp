#include "../Application.h"
#include "../Manager/Generic/SceneManager.h"
#include "../Manager/Generic/InputManager.h"
#include "../Manager/GameSystem/OrderCustomerManager.h"
#include "../Object/Player.h"
#include "../Object/Stage/StageManager.h"
#include "../Object/SkyDome.h"
#include "TutorialScene.h"

TutorialScene::TutorialScene(void)
{
}

TutorialScene::~TutorialScene(void)
{
}

void TutorialScene::Init(void)
{
	//�v���C���[
	player_ = std::make_unique<Player>();
	player_->Init();

	//�X�e�[�W
	stage_ = std::make_unique<StageManager>(*player_);
	stage_->Init();

	//�X�e�[�W�̃R���C�_�[��ǉ�
	player_->AddCollider(stage_->GetTransform().collider);

	//�X�J�C�h�[��
	skyDome_ = std::make_unique<SkyDome>(player_->GetTransform());
	skyDome_->Init();

	//�q�ƒ���
	customer_ = std::make_unique<OrderCustomerManager>();
	customer_->Init();

}

void TutorialScene::Update(void)
{
	//�V�[���J��
	InputManager& ins = InputManager::GetInstance();
	if (ins.IsInputTriggered("NextScene"))
	{
		SceneManager::GetInstance().ChangeScene(SceneManager::SCENE_ID::GAME);
	}
}

void TutorialScene::Draw(void)
{
	DrawString(0, 0, L"�`���[�g���A��", 0xFFFFFF);


	SetFontSize(32);
	int width = GetDrawStringWidth(L"Push Enter", 10);
	DrawString(Application::SCREEN_SIZE_X / 2 - width / 2, 300, L"Push Enter", 0xffffff);
	SetFontSize(16);
}
