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

	//�J�E���g�_�E���p
	static constexpr int  COUNTDOWN_FRAME = 60;	//�U�O�t���[���łP�b

	enum class PHASE 
	{
		COUNT_DOWN,     // �J�E���g�_�E��
		GAME,			// �Q�[����
		FINISH			//�Q�[���I��      
	};
	
	//�R���X�g���N�^
	GameScene(void);

	//�f�X�g���N�^
	~GameScene(void);

	void Init(void) override;
	void Update(void) override;
	void Draw(void) override;

private:

	PHASE phase_;

	//�X�e�[�W
	std::unique_ptr<StageManager> stage_;

	//�v���C���[
	std::unique_ptr<Player> player_;

	//�q
	std::unique_ptr<OrderCustomerManager> customer_;

	//�^�C�}�[
	std::unique_ptr<Timer> timer_;

	//�X�R�A
	int score_;
	//�X�R�A�p�����摜
	int* numbersImgs_;

	int cntDownTimer_;
	int cntDownIdx_;
	//�J�E���g�_�E���p�����摜
	int* countImgs_;

	float scale_;
	float sclTime_;

	void UpdateGame(void);
	void DrawGame(void);

	/// <summary>
	/// �X�R�A���摜�ŕ`�悷��
	/// </summary>
	/// <param name="score">�X�R�A</param>
	void DrawScore(int score);
};
