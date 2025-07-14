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
	//�Q�[���S�̂̐�������
	static constexpr int  MAX_MINUTE_TIME = 2;
	
	//�R���X�g���N�^
	GameScene(void);

	//�f�X�g���N�^
	~GameScene(void);

	void Init(void) override;
	void Update(void) override;
	void Draw(void) override;

private:

	//�X�e�[�W
	std::unique_ptr<StageManager> stage_;

	//�v���C���[
	std::unique_ptr<Player> player_;

	//�q
	std::unique_ptr<OrderCustomerManager> customer_;

	//�^�C�}�[
	std::unique_ptr<Timer> timer_;

	int score_;
};
