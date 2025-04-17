#include<string>
#include "../Manager/SceneManager.h"
#include "../Manager/InputManager.h"
#include "../Object//Score.h"
#include "ResultScene.h"

ResultScene::ResultScene(void)
{
}

ResultScene::~ResultScene(void)
{
}

void ResultScene::Init(void)
{
}

void ResultScene::Update(void)
{
	// シーン遷移
	InputManager& ins = InputManager::GetInstance();
	if (ins.IsTrgDown(KEY_INPUT_SPACE))
	{
		SceneManager::GetInstance().ChangeScene(SceneManager::SCENE_ID::TITLE);
	}
}

void ResultScene::Draw(void)
{
	DrawString(0, 0, "リザルト", 0xFFFFFF);

	// スコアを読み込む
	std::vector<int> scores = Score::GetInstance().LoadScore();
	std::vector<int> topScores = Score::GetInstance().GetTopScores(scores); // 上位5つのスコア
	//int totalScore = Score::GetInstance().GetTotalScore(scores);
	DrawFormatString(0, 20, 0xff0000,"スコア", 0xFFFFFF);
	int y = 40;
	for (size_t i = 0; i < topScores.size(); ++i) {
		std::string scoreText = std::to_string(i + 1) + ". " + std::to_string(topScores[i]);
		DrawString(50, y, scoreText.c_str(), GetColor(255, 255, 255));
		y += 30;
	}
}
