#include<string>
#include "../Common/DebugDrawFormat.h"
#include "../Manager/Generic/SceneManager.h"
#include "../Manager/Generic/InputManager.h"
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
	Score::GetInstance().LoadScore();
}

void ResultScene::Update(void)
{
	//�V�[���J��
	InputManager& ins = InputManager::GetInstance();
	if (ins.IsTrgDown(KEY_INPUT_RETURN))
	{
		SceneManager::GetInstance().ChangeScene(SceneManager::SCENE_ID::TITLE);
	}
}

void ResultScene::Draw(void)
{
	DrawString(0, 0, L"���U���g", 0xFFFFFF);
	auto& scr = Score::GetInstance();
	//�X�R�A��ǂݍ���

	DrawFormatString(0, 20, 0xff0000,L"�X�R�A", 0xFFFFFF);

	int line = 1;	//�s
	int lineHeight = 40;	//�s
	SetFontSize(24);
	DebugDrawFormat::FormatString(L"����̃X�R�A : ��%d", scr.GetCurrentScore(),line,lineHeight);
	DebugDrawFormat::FormatString(L"�S�v���C���[�̑��X�R�A : ��%d", scr.GetAggregateScore(),line,lineHeight);
	for (int i = 0; i < Score::RANKING_NUM; ++i) {
		DebugDrawFormat::FormatString(L"%d �� : ��%d", i + 1, scr.GetRankingScore(i), line, lineHeight);
	}
	SetFontSize(16);
}
