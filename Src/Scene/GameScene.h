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
	static constexpr int  MAX_SECOND_TIME = 0;

	//�c��b�������Ȃ����Ƃ���SE��炷�p�̖ڈ��b��
	static constexpr int  SECOND_SOUND_TIME = 30;		//�c��30�b��SE��炷(�x������SE)
	static constexpr int  SECOND_SOUND_TIME_FAST = 10;	//�c��10�b��SE��炷(��������SE)

	//�J�E���g�_�E��
	static constexpr int  MAX_COUNT_DOWN = 4;

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
	//�t�F�[�Y
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

	//�J�E���g�_�E���p�^�C�}�[
	int cntDownTimer_;
	//�J�E���g�_�E���摜�p�C���f�b�N�X
	int cntDownIdx_;

	//�J�E���g�_�E���p�����摜
	int* countImgs_;

	//�J�E���g�_�E����^�C���A�b�v�̉摜�g�嗦
	float scale_;
	//�C�[�W���O�p�̎���(�x������ƃC�[�W���O���������ɂȂ�j
	float sclTime_;

	//�^�C���A�b�v�p�摜
	int timeUpImg_;

	//�^�C�}�[�̃��}�C���h�pSE�̃t���O
	bool remainderSE_;

	//�֐��|�C���^
	using UpdateFunc_t = void(GameScene::*)();
	using DrawFunc_t = void(GameScene::*)();

	UpdateFunc_t update_;
	DrawFunc_t draw_;

	void UpdateCountDown(void);

	/// <summary>
	/// �Q�[�����̍X�V����
	/// </summary>
	/// <param name="">�J�E���g�_�E���A�J�E���g�A�b�v�ȊO�̏���</param>
	void UpdateGame(void);

	void DrawCountDown(void);

	/// <summary>
	/// �Q�[�����̕`��
	/// </summary>
	/// <param name="">�J�E���g�_�E���A�J�E���g�A�b�v�ȊO�̕`��</param>
	void DrawGame(void);

	/// <summary>
	/// �X�R�A���摜�ŕ`�悷��
	/// </summary>
	/// <param name="score">�X�R�A</param>
	void DrawScore(int score);
};
