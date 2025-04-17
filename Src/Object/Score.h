//#pragma once
#include<vector>

class Score
{
public:

	static constexpr int RANKING_NUM = 5;

	// インスタンスの生成
	static void CreateInstance(void);

	// インスタンスの取得
	static Score& GetInstance(void);

	void Init(void);
	void Update(void);
	void Draw(void);

	// リソースの破棄
	void Destroy(void);

	void SaveScore(int score);
	std::vector<int> LoadScore(void);
	std::vector<int> GetTopScores(const std::vector<int>& score);
	int GetTotalScore(void);
private:
	// シングルトン用インスタンス
	static Score* instance_;

	int score_;
	int addScore_;
	int scoreRank_[5];
	std::vector<int> scores;
};

