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
	std::shared_ptr<Player> player_;

	//客
	std::unique_ptr<OrderCustomerManager> customer_;

	float timer_;

	int score_;

	void DebugDraw(void);
};
