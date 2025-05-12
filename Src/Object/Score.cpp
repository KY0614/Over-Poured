#include <DxLib.h>
#include <algorithm>
#include <iostream>
#include <fstream>
#include <stdio.h>
#include"../Libs/nlohmann/json.hpp"
#include"../Application.h"
#include "Score.h"

Score* Score::instance_ = nullptr;

// 長いのでnamespaceの省略
using json = nlohmann::json;

void Score::CreateInstance(void)
{
	if (instance_ == nullptr)
	{
		instance_ = new Score();
	}
	instance_->Init();
}

Score& Score::GetInstance(void)
{
	if (instance_ == nullptr)
	{
		Score::CreateInstance();
	}
	return *instance_;
}

void Score::Init(void)
{
}

void Score::Destroy(void)
{
	delete instance_;
}

void Score::LoadScore(void)
{
	std::ifstream ifs;
	ifs.open(Application::PATH_SCORE + "score.json");
	if (!ifs)
	{
		// 外部ファイルの読み込み失敗
		return;
	}

	// ファイルストリームからjsonオブジェクトに変換
	json scoreData = json::parse(ifs);
	//jsonオブジェクトから、scoreオブジェクトを取得
	const auto& scoreDatas = scoreData["Scores"];
	// enemyオブジェクトは複数あるはずなので、繰り返し処理
	for (const json& scoreData : scoreDatas)
	{
		// 管理配列に追加
		scores_.emplace_back(currentScore_);
	}

	//スコープが切れる際に 自動的にファイルクローズして貰えますが、
	//明示的にファイルストリームを閉じる
	ifs.close();
}

void Score::SaveScore(int score)
{
	//外部ファイルの読み込み(書込み用)
	std::ofstream ofs;
	ofs.open(Application::PATH_SCORE + "score.json");

	scores_.push_back(score);
	int aggScore = 0;
	aggScore = GetAggregateScore() + score;

	//jsonオブジェクト
	json scoreData;
	//スコア
	scoreData["CurrentScore"].push_back(score);
	scoreData["AggregateScore"].push_back(aggScore);

	//jsonオブジェクト(全体)
	json saveData;
	saveData["ScoreData"] = scoreData;
	
	//jsonオブジェクトをstringに変換
	auto dmp = saveData.dump();
	
	//外部ファイルに書込み
	ofs.write(dmp.c_str(), dmp.size());
	
	//変換と書込みを以下のリダイレクトで省略可
	//ofs << saveData << std::endl;
	// スコープが切れる際に 自動的にファイルクローズして貰えますが、
	// お行儀良く、明示的にファイルストリームを閉じる
	ofs.close();
}

int Score::GetAggregateScore(void) const
{
	std::ifstream ifs;
	ifs.open(Application::PATH_SCORE + "score.json");
	if (!ifs)
	{
		// 外部ファイルの読み込み失敗
		return -1;
	}

	// ファイルストリームからjsonオブジェクトに変換
	json scoreData = json::parse(ifs);

	//jsonオブジェクトから、scoreオブジェクトを取得
	const auto& scoreDatas = scoreData["AggregateScore"];

	ifs.close();

	return 0;
}

void Score::UpdateRanking(void)
{
	//// スコアをvectorにコピー
	//std::vector<int> scoreList;
	//for (const auto& s : scores_) {
	//	scoreList.push_back(s["score"].get<int>());
	//}

	//// ソート
	//std::sort(scoreList.begin(), scoreList.end(), std::greater<int>());

	//// ランキングを更新
	//rankingScores_.clear();
	//for (int i = 0; i < /*std::min((int)*/scoreList.size(), RANKING_NUM; ++i) {
	//	rankingScores_.push_back(scoreList[i]);
	//}
}
