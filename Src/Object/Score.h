//#pragma once

#include<vector>

class Score
{
public:

	static constexpr int RANKING_NUM = 5;

	//インスタンスの生成
	static void CreateInstance(void);

	//インスタンスの取得
	static Score& GetInstance(void);

	void Init(void);

	//リソースの破棄
	void Destroy(void);

	//
	void LoadScore(void);
	//void LoadCurrentScore(void);
	//void LoadAggScore(void);

	//ゲーム終了時のスコア保存用
	void SaveScore(int score);

	int GetCurrentScore(void)const { return currentScore_; }

	//全スコアを加算した結果を取得
	int GetAggregateScore(void)const;

	//ランキング順のスコアを取得
	//const std::vector<int>& GetRankingScore(void)const { return rankingScores_; }

	void SetCurrentScore(int score) { currentScore_ = score; }

	void AddScore(int score) { addScore_ += score; }

private:

	//シングルトン用インスタンス
	static Score* instance_;

	//スコア	
	std::vector<int> scores_;
	int currentScore_;		//今回のスコア
	int addScore_;	//全てのスコアを加算したスコア
	int scoreRank_[6];	//ランキング形式のスコア
	int scoreSort_[6];	//ソート用のスコア

	void UpdateRanking(void);
};

