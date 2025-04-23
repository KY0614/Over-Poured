#pragma once
#include <memory>
#include "SceneBase.h"

class Stage;
class SkyDome;
class Player;
class OrderManager;
class CustomerManager;

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
	std::unique_ptr<Stage> stage_;

	//スカイドーム
	std::unique_ptr<SkyDome> skyDome_;

	//プレイヤー
	std::shared_ptr<Player> player_;

	//客
	std::shared_ptr<CustomerManager> customer_;

	float timer_;
	int score_;

	int num;

	void DebugDraw(void);
};
