#include "../Manager/GameSystem/SoundManager.h"
#include "../Manager/Generic/ResourceManager.h"
#include "../Object/Score.h"
#include "ResultScene.h"

ResultScene::ResultScene(void)
{
}

ResultScene::~ResultScene(void)
{
}

void ResultScene::Init(void)
{
	//BGMÇÃçƒê∂
	auto& sound = SoundManager::GetInstance();
	sound.Add(SoundManager::TYPE::BGM, SoundManager::SOUND::RESULT,
		ResourceManager::GetInstance().Load(ResourceManager::SRC::RESULT_BGM).handleId_);
	sound.AdjustVolume(SoundManager::SOUND::RESULT, VOLUME_MAX / 2);
	sound.Play(SoundManager::SOUND::RESULT);

	//ÉXÉRÉA
	score_ = std::make_unique<Score>();
	score_->Init();
}

void ResultScene::Update(void)
{
	score_->Update();
}

void ResultScene::Draw(void)
{
	score_->Draw();
}
