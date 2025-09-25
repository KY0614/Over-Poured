#pragma once
#include "../Manager/GameSystem/ScoreManager.h"
#include "../Common/Vector2.h"

class Score
{
public:
	//ランクの数
	static constexpr int RANK_NUM = 4;		

	struct RankInfo 
	{
		int startVal_ = 0, endVal_ = 0;	//ランクごとの上限下限値
		float currentRate_ = 0.0f;		//
		float displayedRate_ = 0.0f;	//
		int gaugeImg_ = -1;		//ゲージ画像	
		bool isFull_ = false;
	};

	//ランクの種類
	enum class RANK 
	{
		C,
		B,
		A,
		S,
		MAX
	};

	//スコア画面の状態
	enum class STATE
	{
		PLAY_SCORE,		//今回のスコアとランキング表示
		TOTAL_SCORE		//総スコア表示
	};

	// コンストラクタ
	Score(void);

	// デストラクタ
	~Score(void);

	/// <summary>
	/// 初期化処理
	/// </summary>
	void Init(void);

	/// <summary>
	/// 更新処理
	/// </summary>
	void Update(void);

	/// <summary>
	/// 描画処理
	/// </summary>
	void Draw(void);

private:
	//スコア画面の状態
	STATE state_;

	//今回のゲームのランク
	RANK rank_;
	
	//状態管理(状態遷移時初期処理)
	std::map<STATE, std::function<void(void)>> stateChange_;
	//状態管理(状態更新処理)
	std::function<void(void)> stateUpdate_;
	//状態管理(状態描画処理)
	std::function<void(void)> stateDraw_;

	//現在スコアを描画済みかどうか
	bool isCurrentScrDraw_;	//true:描画済み false:未描画

	//ランキングスコアを描画済みかどうか
	bool isRankingScrDraw_;	//true:描画済み false:未描画

	//ゲージを描画済みかどうか
	bool isGaugeDraw_;		//true:描画済み false:未描画

	//今回のゲームのスコア
	int currentScr_;

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

	//ランキングのラベル画像（１位：～５位：）
	int* rankLabelImgs_;
	//ランキングの背景画像
	int rankingBackImg_;

	//数字用
	int* numberImgs_;
	//「現在スコア」ラベル画像
	int currentScrImg_;
	//ランクごとの文字画像（C,B,A,S)
	int* ranksImgs_;
	//ゲージ画像
	int circleShadowImg_;
	//背景の装飾画像
	int decoImg_;
	//push space or Button画像
	int pushImg_;	
	
	//画面比率
	float aspectRatio_;

	/// <summary>
	/// 現在スコアを０から足しながら描画
	/// </summary>
	/// <param name="score">現在スコア</param>
	/// <param name="posX">X座標</param>
	/// <param name="posY">Y座標</param>
	/// <param name="scale">1文字あたりの大きさ</param>
	void DrawVariableScore(int score,int posX,int posY, float scale);

	/// <summary>
	/// ランキングスコアを描画
	/// </summary>
	/// <param name="score">現在スコア</param>
	/// <param name="posX">X</param>
	/// <param name="posY">Y座標</param>
	/// <param name="hightLight">ハイライト</param>
	void DrawRankingScore(int score,int posX,int posY,int hightLight);

	void DrawScore(int score, int posX, int posY);

	void ChangeState(STATE state);
	void ChangePlayScore(void);

	void UpdatePlayScore(void);

	void DrawPlayScore(void);

	void CalcPercentFromRank(void);

	RANK GetRankFromScore(int score);

	void InitRankInfo(void);

	void LoadImages(void);

	void InitSound(void);
};

