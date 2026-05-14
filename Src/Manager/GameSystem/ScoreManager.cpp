#include <DxLib.h>
#include <fstream>
#include"../Libs/nlohmann/json.hpp"
#include"../Application.h"
#include "ScoreManager.h"

ScoreManager* ScoreManager::instance_ = nullptr;

// 長いのでnamespaceの省略
using json = nlohmann::json;

void ScoreManager::CreateInstance(void)
{
	if (instance_ == nullptr)
	{
		instance_ = new ScoreManager();
	}
	instance_->Init();
}

ScoreManager& ScoreManager::GetInstance(void)
{
	if (instance_ == nullptr)
	{
		ScoreManager::CreateInstance();
	}
	return *instance_;
}

void ScoreManager::Init(void)
{
	scores_.clear();
	currentScore_ = 0;
	for(int i = 0; i < RANKING_NUM; ++i) 
	{
		scoreRank_[i] = 0;
	}
}

void ScoreManager::Destroy(void)
{
	delete instance_;
}

void ScoreManager::LoadScore(void)
{
	scores_.clear();
	std::ifstream ifs(Application::PATH_SCORE + "score.json");
	if (!ifs)return;//読み込み失敗

	// ファイルストリームからjsonオブジェクトに変換
	json scoreData = json::parse(ifs);

	//jsonオブジェクトから、scoreオブジェクトを取得
	const auto& scoreDatas = scoreData["Scores"];
	// scoreオブジェクトは複数あるはずなので、繰り返し処理
	for (const json& s : scoreDatas)
	{
		// 管理配列に追加
		scores_.emplace_back(s.get<int>());
	}

	//スコープが切れる際に 自動的にファイルクローズして貰えますが、
	//明示的にファイルストリームを閉じる
	ifs.close();

	SortRankingScore(); //ランキングも更新
}

void ScoreManager::SaveScore(const int score)
{
	//管理配列に追加
	const std::string scoreFileName = "score.json";
	std::ifstream ifs(Application::PATH_SCORE + scoreFileName);
	json scoreData;
	//ファイルが存在する場合は読み込み
	if (ifs)
	{
		scoreData = json::parse(ifs);
		ifs.close();
	}
	const std::string scoreKey = "Scores";
	//存在しない場合は新規作成
	if (!scoreData.contains(scoreKey))
	{
		scoreData[scoreKey] = json::array();
	}
	//管理配列に追加
	scoreData[scoreKey].push_back(score);
	
	const int dumpIndent = 4; //インデントのスペース数
	std::ofstream ofs(Application::PATH_SCORE + scoreFileName);
	ofs << scoreData.dump(dumpIndent);
	ofs.close();
}

void ScoreManager::SortRankingScore(void)
{
	//スコアをソートする
	std::vector<int> scoreList = scores_;
	std::sort(scoreList.begin(), scoreList.end(), std::greater<int>());

	//５位までのスコアを降順で入れる(スコアが５つ以上ない場合は０を入れる)
	for (int i = 0; i < RANKING_NUM && i < scoreList.size(); ++i) 
	{
		scoreRank_[i] = scoreList[i];
	}
	for (int i = static_cast<int>(scoreList.size()); i < RANKING_NUM; ++i)
	{
		scoreRank_[i] = 0;
	}
}

ScoreManager::ScoreManager(void)
{
	scores_ = {};
	currentScore_ = 0;	
	for(int i = 0; i < RANKING_NUM;i++) 
	{
		scoreRank_[i] = 0;
	}
}