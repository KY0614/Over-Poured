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
	void Update(void);
	void Draw(void);

	//リソースの破棄
	void Destroy(void);

	//
	void LoadScore(void);

	//ゲーム終了時のスコア保存用
	void SaveScore(int score);

	//全スコアを加算した結果を取得
	void GetAggregateScore(void);

	//ランキング順のスコアを取得
	void GetRankingScore(void);

	void ScoreSort(void);

	void AddScore(int score) { addScore_ += score; }

private:

	//シングルトン用インスタンス
	static Score* instance_;

	//スコア用の外部ファイル関連
	FILE* file;		//ファイル構造体
	errno_t err;	//エラー用

	//スコア	
	int score_;		//今回のスコア
	int addScore_;	//全てのスコアを加算したスコア
	int scoreRank_[6];	//ランキング形式のスコア
	int scoreSort_[6];	//ソート用のスコア
};

