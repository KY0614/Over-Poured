#pragma once
#include <memory>
#include "SceneBase.h"

class StageManager;
class Player;
class OrderCustomerManager;
class Timer;

class TutorialScene : public SceneBase
{
public:

	//コンストラクタ
	TutorialScene(void);

	//デストラクタ
	~TutorialScene(void);

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
};

