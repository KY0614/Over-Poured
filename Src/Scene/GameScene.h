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
	static constexpr int  MAX_MINUTE_TIME = 2;			//分
	static constexpr int  MAX_SECOND_TIME = 0;			//秒

	//残り秒数が少なったときにSEを鳴らす用の目安秒数
	static constexpr int  SECOND_SOUND_TIME = 30;		//残り30秒でSEを鳴らす(遅い方のSE)
	static constexpr int  SECOND_SOUND_TIME_FAST = 10;	//残り10秒でSEを鳴らす(速い方のSE)

	//カウントダウン画像の枚数
	static constexpr int  MAX_COUNT_DOWN = 4;			//start, 1, 2, 3の4枚

	//カウントダウン画像を拡大する時間
	static constexpr float  CNTDOWN_SCL_MAX_TIME = 1.0f;//拡大にかかる最大時間

	//カウントダウン画像の拡大率
	static constexpr float  CNTDOWN_IMG_MAX_SCL = 2.0f;	//最大拡大率
	
	//コンストラクタ
	GameScene(void);

	//デストラクタ
	~GameScene(void);

	void Init(void) override;
	void Update(void) override;
	void Draw(void) override;

private:
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

	//関数ポインタ（カウントダウン、ゲーム中、タイムアップ）
	using UpdateFunc_t = void(GameScene::*)();
	using DrawFunc_t = void(GameScene::*)();

	UpdateFunc_t update_;
	DrawFunc_t draw_;

	/// <summary>
	/// カウントダウン時の更新処理
	/// </summary>
	/// <param name="">カウントダウン画像のイージング処理と拡大処理</param>
	void UpdateCountDown(void);

	/// <summary>
	/// ゲーム中の更新処理
	/// </summary>
	/// <param name="">ゲーム中の処理</param>
	void UpdateGame(void);

	/// <summary>
	/// タイムアップ時の更新処理
	/// </summary>
	/// <param name=""></param>
	void UpdateFinish(void);

	/// <summary>
	/// カウントダウン時の描画処理
	/// </summary>
	/// <param name=""></param>
	void DrawCountDown(void);

	/// <summary>
	/// ゲーム中の描画
	/// </summary>
	/// <param name="">カウントダウン、カウントアップ以外の描画</param>
	void DrawGame(void);

	/// <summary>
	/// タイムアップ時の描画処理
	/// </summary>
	/// <param name=""></param>
	void DrawFinish(void);

	/// <summary>
	/// スコアを画像で描画する
	/// </summary>
	/// <param name="score">スコア</param>
	void DrawScore(int score);
};
