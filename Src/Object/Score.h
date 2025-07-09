#pragma once
#include "../Manager/GameSystem/ScoreManager.h"
#include "../Common/Vector2.h"

class Score
{
public:
	//スコア加算スピード
	static constexpr int ADD_SCORE_SPEED = 8;

	//イージング関連
	static constexpr float START_SLIDE_X = -300.0f;	//開始位置
	static constexpr float END_SLIDE_X = 100.0f;		//終了位置
	static constexpr float NEXT_SLIDE_START_X = -125.0f;		//次のイージングを開始する目標位置
	static constexpr float SLIDE_MAX_TIME = 1.0f;	//目標時間

	static constexpr int RANK_NUM = 4;
	static constexpr int RANK_C_MAX = 499;
	static constexpr int RANK_B_MAX = 999;
	static constexpr int RANK_A_MAX = 1499;
	static constexpr int RANK_S_MAX = 1999;
	
	struct RankInfo {
		int startVal_ = 0, endVal_ = 0;	//ランクごとの上限下限値
		float currentRate_ = 0.0f;			//
		float displayedRate_ = 0.0f;		//
		int gaugeImg_ = -1;	//ゲージ画像	
		Vector2 pos_ = {0,0};	//表示座標
	};

	enum class RANK {
		C,
		B,
		A,
		S,
		MAX
	};

	enum class STATE
	{
		PLAY_SCORE,
		TOTAL_SCORE
	};

	// コンストラクタ
	Score(void);

	// デストラクタ
	~Score(void);

	void Init(void);
	void Update(void);
	void Draw(void);

private:
	STATE state_;

	RANK rank_;

	std::map<STATE, std::function<void(void)>> stateChange_;

	std::function<void(void)> stateUpdate_;
	std::function<void(void)> stateDraw_;

	//今回のゲームのスコア
	int currentScr_;

	//ランキングスコアを滑らかに表示させるよう
	float slideX_[ScoreManager::RANKING_NUM];		//移動させる座標
	float slideTime_[ScoreManager::RANKING_NUM];	//経過時間
	bool isMove_[ScoreManager::RANKING_NUM];		//移動中かどうか（ディレイをかけて表示する用）
	
	//ハイライト点滅表示用インデックス
	int highLightIdx_;	
	//ハイライト点滅を繰り返す用の時間
	float blinkTime_;

	//スコアによるランク表示用
	RankInfo rankData_[RANK_NUM];

	int circleShadowImg_;

	void DebugDraw(void);

	void ChangeState(STATE state);
	void ChangePlayScore(void);
	void ChangeTotalScore(void);

	void UpdatePlayScore(void);
	void UpdateTotalScore(void);

	void DrawPlayScore(void);
	void DrawTotalScore(void);

	void CalcPercentFromRank(void);

	RANK GetRankFromScore(int score);

	void InitRankInfo(void);

};

