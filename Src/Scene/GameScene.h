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
		NONE,			// �������p
		GAME_3D,		// 3D�Q�[�����[�h
		MACHINE_2D,		// 2D�}�V�����[�h
		ICE_2D,			// 2D���X�@���[�h
		LIDRACK_2D,		// 2D���b�h���b�N���[�h
	};
	
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

	//�X�J�C�h�[��
	std::unique_ptr<SkyDome> skyDome_;

	//�v���C���[
	std::unique_ptr<Player> player_;

	//�q
	std::unique_ptr<OrderCustomerManager> customer_;

	MODE mode_;

	//��ԊǗ�(��ԑJ�ڎ���������)
	std::map<MODE, std::function<void(void)>> stateChanges_;

	//��ԊǗ�(�X�V�X�e�b�v)
	std::function<void(void)> stateUpdate_;

	float timer_;

	int score_;

	//��ԑJ��
	void ChangeMode(MODE mode);
	void ChangeMode3DGame(void);
	void ChangeModeMachine2D(void);
	void ChangeModeIce2D(void);
	void ChangeModeLidRack2D(void);

	//�X�V�X�e�b�v
	void UpdateNone(void);
	void Update3DGame(void);
	void UpdateMachine2D(void);
	void UpdateIce2D(void);
	void UpdateLidRack2D(void);

	void DebugDraw(void);
};
