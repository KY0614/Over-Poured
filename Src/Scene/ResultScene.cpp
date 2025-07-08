#include<string>
#include "../Common/DebugDrawFormat.h"
#include "../Manager/Generic/SceneManager.h"
#include "../Manager/Generic/InputManager.h"
#include "../Object/Score.h"
#include "ResultScene.h"

ResultScene::ResultScene(void)
{
	currentScr_ = 0;
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
	auto& scr = Score::GetInstance();
	if (currentScr_ >= scr.GetCurrentScore())
	{
		currentScr_ = scr.GetCurrentScore();
	}
	else 
	{
		currentScr_ += ADD_SCORE_SPEED;
	}
	//シーン遷移
	InputManager& ins = InputManager::GetInstance();
	if (ins.IsInputTriggered("NextScene"))
	{
		SceneManager::GetInstance().ChangeScene(SceneManager::SCENE_ID::TITLE);
	}
}

void ResultScene::Draw(void)
{
	DrawString(0, 0, L"リザルト", 0xFFFFFF);
	auto& scr = Score::GetInstance();
	//スコアを読み込む

	DrawFormatString(0, 20, 0xff0000,L"スコア", 0xFFFFFF);

	int line = 1;	//行
	int lineHeight = 40;	//行
	SetFontSize(24);
	DebugDrawFormat::FormatStringRight(L"今回のスコア : ￥%d      ", currentScr_,line,lineHeight);
	DebugDrawFormat::FormatString(L"全プレイヤーの総スコア : ￥%d", scr.GetAggregateScore(),line,lineHeight);
	for (int i = 0; i < Score::RANKING_NUM; ++i) {
		DebugDrawFormat::FormatString(L"%d 位 : ￥%d", i + 1, scr.GetRankingScore(i), line, lineHeight);
	}
	SetFontSize(16);
}
