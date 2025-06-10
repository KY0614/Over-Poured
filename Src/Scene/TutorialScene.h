#pragma once
#include <memory>
#include "SceneBase.h"

class StageManager;
class SkyDome;
class Player;
class OrderCustomerManager;

class TutorialScene : public SceneBase
{
public:

	//�R���X�g���N�^
	TutorialScene(void);

	//�f�X�g���N�^
	~TutorialScene(void);

	void Init(void) override;
	void Update(void) override;
	void Draw(void) override;

private:

	//�X�e�[�W
	std::unique_ptr<StageManager> stage_;

	//�X�J�C�h�[��
	std::unique_ptr<SkyDome> skyDome_;

	//�v���C���[
	std::unique_ptr<Player> player_;

	//�q
	std::unique_ptr<OrderCustomerManager> customer_;

	float timer_;
};

