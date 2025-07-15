#pragma once
#include <memory>
#include "SceneBase.h"

class StageManager;
class Player;
class OrderManager;
class OrderCustomerManager;
class Timer;

class GameScene : public SceneBase
{

public:
	//ゲーム全体の制限時間
	static constexpr int  MAX_MINUTE_TIME = 2;

	//カウントダウン用
	static constexpr int  COUNTDOWN_FRAME = 60;	//６０フレームで１秒

	enum class PHASE 
	{
		COUNT_DOWN,     // カウントダウン
		GAME,			// ゲーム中
		FINISH			//ゲーム終了      
	};
	
	//コンストラクタ
	GameScene(void);

	//デストラクタ
	~GameScene(void);

	void Init(void) override;
	void Update(void) override;
	void Draw(void) override;

private:

	PHASE phase_;

	//ステージ
	std::unique_ptr<StageManager> stage_;

	//プレイヤー
	std::unique_ptr<Player> player_;

	//客
	std::unique_ptr<OrderCustomerManager> customer_;

	//タイマー
	std::unique_ptr<Timer> timer_;

	//スコア
	int score_;
	//スコア用数字画像
	int* numbersImgs_;

	int cntDownTimer_;
	int cntDownIdx_;
	//カウントダウン用数字画像
	int* countImgs_;

	float scale_;
	float sclTime_;

	void UpdateGame(void);
	void DrawGame(void);

	/// <summary>
	/// スコアを画像で描画する
	/// </summary>
	/// <param name="score">スコア</param>
	void DrawScore(int score);
};
