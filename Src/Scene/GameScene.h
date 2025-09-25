#pragma once
#include <memory>
#include "SceneBase.h"

class StageManager;
class Player;
class OrderCustomerManager;
class Timer;

class GameScene : public SceneBase
{
public:

	//コンストラクタ
	GameScene(void);

	//デストラクタ
	~GameScene(void);

	/// <summary>
	/// 初期化処理
	/// </summary>
	void Init(void) override;

	/// <summary>
	/// 更新処理
	/// </summary>
	void Update(void) override;

	/// <summary>
	/// 描画処理
	/// </summary>
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

	//関数ポインタ変数
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

	/// <summary>
	/// 画像の読み込み処理
	/// </summary>
	void LoadImages(void);

	/// <summary>
	/// サウンドの初期化処理
	/// </summary>
	void InitSound(void);
};
