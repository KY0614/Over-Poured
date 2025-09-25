//#pragma once

#include<vector>

class ScoreManager
{
public:
	//ランキングの数
	static constexpr int RANKING_NUM = 5;

	//インスタンスの生成
	static void CreateInstance(void);

	//インスタンスの取得
	static ScoreManager& GetInstance(void);

	void Init(void);

	//リソースの破棄
	void Destroy(void);

	/// <summary>
	/// スコアの読み込み
	/// </summary>
	void LoadScore(void);

	/// <summary>
	/// ゲーム終了時のスコアをjsonに保存
	/// </summary>
	/// <param name="score">保存するスコア</param>
	void SaveScore(const int score);

	/// <summary>
	/// 今回のスコアを取得
	/// </summary>
	/// <returns>今回のスコア</returns>
	const int& GetCurrentScore(void)const { return currentScore_; }

	/// <summary>
	/// 全スコアを加算した結果を取得
	/// </summary>
	/// <param name=""></param>
	/// <returns>全スコアを加算した結果</returns>
	const int& GetAggregateScore(void)const;

	//ランキング順のスコアを取得

	/// <summary>
	/// 指定した要素のランキングのスコアを取得します。
	/// </summary>
	/// <param name="element">ランキングスコアを取得する要素のインデックス</param>
	/// <returns>指定した要素のランキングスコアへの参照。</returns>
	const int& GetRankingScore(int element)const { return scoreRank_[element]; }

	/// <summary>
	/// 現在のスコアを設定します。
	/// </summary>
	/// <param name="score">設定するスコアの値</param>
	void SetCurrentScore(const int score) { currentScore_ = score; }

private:

	//シングルトン用インスタンス
	static ScoreManager* instance_;

	//スコア	
	std::vector<int> scores_;
	int currentScore_;		//今回のスコア
	int scoreRank_[6];	//ランキング形式のスコア

	/// <summary>
	/// スコアをソートする
	/// </summary>
	void SortRankingScore(void);
};

