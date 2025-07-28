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
	static constexpr int  MAX_SECOND_TIME = 0;

	//残り秒数が少なったときにSEを鳴らす用の目安秒数
	static constexpr int  SECOND_SOUND_TIME = 30;		//残り30秒でSEを鳴らす(遅い方のSE)
	static constexpr int  SECOND_SOUND_TIME_FAST = 10;	//残り10秒でSEを鳴らす(速い方のSE)

	//カウントダウン
	static constexpr int  MAX_COUNT_DOWN = 4;

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
	//フェーズ
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

	//カウントダウン用タイマー
	int cntDownTimer_;
	//カウントダウン画像用インデックス
	int cntDownIdx_;

	//カウントダウン用数字画像
	int* countImgs_;

	//カウントダウンやタイムアップの画像拡大率
	float scale_;
	//イージング用の時間(遅くするとイージングがゆっくりになる）
	float sclTime_;

	//タイムアップ用画像
	int timeUpImg_;

	//タイマーのリマインド用SEのフラグ
	bool remainderSE_;

	//関数ポインタ
	using UpdateFunc_t = void(GameScene::*)();
	using DrawFunc_t = void(GameScene::*)();

	UpdateFunc_t update_;
	DrawFunc_t draw_;

	void UpdateCountDown(void);

	/// <summary>
	/// ゲーム中の更新処理
	/// </summary>
	/// <param name="">カウントダウン、カウントアップ以外の処理</param>
	void UpdateGame(void);

	void DrawCountDown(void);

	/// <summary>
	/// ゲーム中の描画
	/// </summary>
	/// <param name="">カウントダウン、カウントアップ以外の描画</param>
	void DrawGame(void);

	/// <summary>
	/// スコアを画像で描画する
	/// </summary>
	/// <param name="score">スコア</param>
	void DrawScore(int score);
};
