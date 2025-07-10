#pragma once
#include "../Manager/GameSystem/ScoreManager.h"
#include "../Common/Vector2.h"

class Score
{
public:
	//スコア加算スピード
	static constexpr int ADD_SCORE_SPEED = 8;
	static constexpr int ADD_CURRENT_SPEED = 5;
	static constexpr int ADD_TOTALSCORE_SPEED = 123;

	//イージング関連
	static constexpr float START_SLIDE_X = -300.0f;	//X開始位置
	static constexpr float END_SLIDE_X = 100.0f;	//X終了位置
	static constexpr float START_SLIDE_Y = 690.0f;	//Y開始位置
	static constexpr float END_SLIDE_Y = 420.0f;	//Y終了位置
	static constexpr float NEXT_SLIDE_START_X = -125.0f;	//次のイージングを開始する目標位置
	static constexpr float SLIDE_MAX_TIME = 1.0f;	//目標時間
	static constexpr float BLINK_SPEED = 0.5f;		//ハイライト点滅の間隔

	static constexpr float MAX_GAUGE_TIME = 1.0f;		//ゲージのイージング目標時間
	static constexpr float FIRST_GAUGE_SPEED = 0.5f;	//最初に表示するゲージのスピード
	static constexpr float CURRENT_GAUGE_SPEED = 0.5f;	//最後に表示するゲージのスピード
	
	//ランク関連
	static constexpr int RANK_NUM = 4;		//ランクの数
	static constexpr int RANK_C_MAX = 500;	//Cランクの最大値
	static constexpr int RANK_B_MAX = 1000;	//Bランクの最大値
	static constexpr int RANK_A_MAX = 1500;	//Aランクの最大値
	static constexpr int RANK_S_MAX = 1999;	//Sランクの最大値

	static constexpr int RANK_C_MIN = 0;	//Cランクの最小値
	static constexpr int RANK_B_MIN = 501;	//Bランクの最小値
	static constexpr int RANK_A_MIN = 1001;	//Aランクの最小値
	static constexpr int RANK_S_MIN = 1501;	//Sランクの最小値
	
	struct RankInfo {
		int startVal_ = 0, endVal_ = 0;	//ランクごとの上限下限値
		float currentRate_ = 0.0f;		//
		float displayedRate_ = 0.0f;	//
		int gaugeImg_ = -1;		//ゲージ画像	
		bool isFull_ = false;
	};

	enum class RANK {
		C,
		B,
		A,
		S,
		MAX
	};

	enum class TOTALSCR_PHASE {
		COUNT_UP,       // カウントアップ中
		SHOW_CURRENT,   // 今回のスコアを別表示
		MOVE_TO_TOTAL,  // 移動中
		MERGE,          // 合体演出
		FINISH          // 終了
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
	TOTALSCR_PHASE phase_;

	std::map<STATE, std::function<void(void)>> stateChange_;

	std::function<void(void)> stateUpdate_;
	std::function<void(void)> stateDraw_;

	//今回のゲームのスコア
	int currentScr_;

	//全プレイヤーの総スコア
	int totalScr_;

	//ランキングスコアを滑らかに表示させるよう
	float slideX_[ScoreManager::RANKING_NUM];		//移動させる座標
	float slideXTime_[ScoreManager::RANKING_NUM];	//経過時間
	bool isMove_[ScoreManager::RANKING_NUM];		//移動中かどうか（ディレイをかけて表示する用）
	
	//ハイライト点滅表示用インデックス
	int highLightIdx_;	
	//ハイライト点滅を繰り返す用の時間
	float blinkTime_;

	//スコアによるランク表示用
	RankInfo rankData_[RANK_NUM];

	//ゲージを進ませる用の時間
	float gaugeTime_;
	//現在ゲージを進ませているランクのインデックス
	int currentRankIdx_;

	float slideY_;
	float slideYTime_;

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

