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
	scores_.clear();
	std::ifstream ifs(Application::PATH_SCORE + "score.json");
	if (!ifs)return;//読み込み失敗

	// ファイルストリームからjsonオブジェクトに変換
	json scoreData = json::parse(ifs);

	//jsonオブジェクトから、scoreオブジェクトを取得
	const auto& scoreDatas = scoreData["Scores"];
	// enemyオブジェクトは複数あるはずなので、繰り返し処理
	for (const json& s : scoreDatas)
	{
		// 管理配列に追加
		scores_.emplace_back(s.get<int>());
	}

	//スコープが切れる際に 自動的にファイルクローズして貰えますが、
	//明示的にファイルストリームを閉じる
	ifs.close();

	UpdateRanking(); //ランキングも更新
}

void Score::SaveScore(int score)
{
	////外部ファイルの読み込み(書込み用)
	//std::ofstream ofs;
	//ofs.open(Application::PATH_SCORE + "score.json");

	//scores_.push_back(score);
	//int aggScore = 0;
	//aggScore = GetAggregateScore() + score;

	////jsonオブジェクト
	//json scoreData;
	////スコア
	//scoreData["CurrentScore"].push_back(score);
	//scoreData["AggregateScore"].push_back(aggScore);

	////jsonオブジェクト(全体)
	//json saveData;
	//saveData["ScoreData"] = scoreData;
	//
	////jsonオブジェクトをstringに変換
	//auto dmp = saveData.dump();
	//
	////外部ファイルに書込み
	//ofs.write(dmp.c_str(), dmp.size());
	//
	////変換と書込みを以下のリダイレクトで省略可
	////ofs << saveData << std::endl;
	//// スコープが切れる際に 自動的にファイルクローズして貰えますが、
	//// お行儀良く、明示的にファイルストリームを閉じる
	//ofs.close();

	std::ifstream ifs(Application::PATH_SCORE + "score.json");
	json scoreData;
	if (ifs)
	{
		scoreData = json::parse(ifs);
		ifs.close();
	}

	if (!scoreData.contains("Scores")) {
		scoreData["Scores"] = json::array();
	}

	scoreData["Scores"].push_back(score);

	std::ofstream ofs(Application::PATH_SCORE + "score.json");
	ofs << scoreData.dump(4);
	ofs.close();
}

int Score::GetAggregateScore(void) const
{
	int total = 0;
	for (int s : scores_) {
		total += s;
	}
	return total;
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

	std::vector<int> scoreList = scores_;
	std::sort(scoreList.begin(), scoreList.end(), std::greater<int>());

	for (int i = 0; i < RANKING_NUM && i < scoreList.size(); ++i) {
		scoreRank_[i] = scoreList[i];
	}
	for (int i = scoreList.size(); i < RANKING_NUM; ++i) {
		scoreRank_[i] = 0;
	}
}
