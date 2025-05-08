#include <DxLib.h>
#include <string>
#include <fstream>
#include <algorithm>
#include <numeric>
#include <stdio.h>
#include"../Libs/nlohmann/json.hpp"
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

void Score::Update(void)
{
}

void Score::Draw(void)
{
}

void Score::Destroy(void)
{
	delete instance_;
}

void Score::LoadScore(void)
{
	char buffer[256];
	err = fopen_s(&file, "Data/Score/score.txt", "r");
	if (err != 0) { //エラー発生
		return;
	}
	// ファイルの内容を読み込んで表示
	while (fgets(buffer, sizeof(buffer), file) != NULL) {
		printf("%s", buffer);
	}
	fclose(file);
}

void Score::SaveScore(int score)
{
	err = fopen_s(&file, "Data/Score/score.txt", "w");
	if (err != 0) { //エラー発生
		return;
	}
	addScore_ += score;

	fprintf(file, "今回のゲームのスコア ： %d\n", score);

	fclose(file);

	// 外部ファイルの読み込み(書込み用)
	std::ofstream ofs;
	ofs.open("Data/Score/score.json");

	// jsonオブジェクト
	json scoreData;
	//スコア
	scoreData["Score"].push_back(score);
	scoreData["AddScore"].push_back(addScore_);

	// jsonオブジェクト(全体)
	json saveData;
	saveData["ScoreData"] = scoreData;
	
	// jsonオブジェクトをstringに変換
	auto dmp = saveData.dump();
	
	// 外部ファイルに書込み
	ofs.write(dmp.c_str(), dmp.size());
	
	// 変換と書込みを以下のリダイレクトで省略可
	//ofs << saveData << std::endl;
	// スコープが切れる際に 自動的にファイルクローズして貰えますが、
	// お行儀良く、明示的にファイルストリームを閉じる
	ofs.close();

}

void Score::GetAggregateScore(void)
{
}

void Score::GetRankingScore(void)
{
}

void Score::ScoreSort(void)
{
}
