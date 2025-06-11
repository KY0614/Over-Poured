#pragma once
#include <memory>
#include "SceneBase.h"

class StageManager;
class SkyDome;
class Player;
class OrderManager;
class OrderCustomerManager;

class GameScene : public SceneBase
{

public:

	static constexpr float MAX_TIME = 50.0f;

	enum class MODE {
		NONE,			// 初期化用
		GAME_3D,		// 3Dゲームモード
		MACHINE_2D,		// 2Dマシンモード
		ICE_2D,			// 2D製氷機モード
		LIDRACK_2D,		// 2Dリッドラックモード
	};
	
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

	//スカイドーム
	std::unique_ptr<SkyDome> skyDome_;

	//プレイヤー
	std::unique_ptr<Player> player_;

	//客
	std::unique_ptr<OrderCustomerManager> customer_;

	MODE mode_;

	//状態管理(状態遷移時初期処理)
	std::map<MODE, std::function<void(void)>> stateChanges_;

	//状態管理(更新ステップ)
	std::function<void(void)> stateUpdate_;

	float timer_;

	int score_;

	//状態遷移
	void ChangeMode(MODE mode);
	void ChangeMode3DGame(void);
	void ChangeModeMachine2D(void);
	void ChangeModeIce2D(void);
	void ChangeModeLidRack2D(void);

	//更新ステップ
	void UpdateNone(void);
	void Update3DGame(void);
	void UpdateMachine2D(void);
	void UpdateIce2D(void);
	void UpdateLidRack2D(void);

	void DebugDraw(void);
};
