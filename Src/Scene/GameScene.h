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
	static constexpr int  MAX_MINUTE_TIME = 2;			//��
	static constexpr int  MAX_SECOND_TIME = 0;			//�b

	//�c��b�������Ȃ����Ƃ���SE��炷�p�̖ڈ��b��
	static constexpr int  SECOND_SOUND_TIME = 30;		//�c��30�b��SE��炷(�x������SE)
	static constexpr int  SECOND_SOUND_TIME_FAST = 10;	//�c��10�b��SE��炷(��������SE)

	//�J�E���g�_�E���摜�̖���
	static constexpr int  MAX_COUNT_DOWN = 4;			//start, 1, 2, 3��4��

	//�J�E���g�_�E���摜���g�傷�鎞��
	static constexpr float  CNTDOWN_SCL_MAX_TIME = 1.0f;//�g��ɂ�����ő厞��

	//�J�E���g�_�E���摜�̊g�嗦
	static constexpr float  CNTDOWN_IMG_MAX_SCL = 2.0f;	//�ő�g�嗦
	
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

	//�X�R�A
	int score_;

	//�X�R�A�p�����摜
	int* numbersImgs_;

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

	//�֐��|�C���^�i�J�E���g�_�E���A�Q�[�����A�^�C���A�b�v�j
	using UpdateFunc_t = void(GameScene::*)();
	using DrawFunc_t = void(GameScene::*)();

	UpdateFunc_t update_;
	DrawFunc_t draw_;

	/// <summary>
	/// �J�E���g�_�E�����̍X�V����
	/// </summary>
	/// <param name="">�J�E���g�_�E���摜�̃C�[�W���O�����Ɗg�又��</param>
	void UpdateCountDown(void);

	/// <summary>
	/// �Q�[�����̍X�V����
	/// </summary>
	/// <param name="">�Q�[�����̏���</param>
	void UpdateGame(void);

	/// <summary>
	/// �^�C���A�b�v���̍X�V����
	/// </summary>
	/// <param name=""></param>
	void UpdateFinish(void);

	/// <summary>
	/// �J�E���g�_�E�����̕`�揈��
	/// </summary>
	/// <param name=""></param>
	void DrawCountDown(void);

	/// <summary>
	/// �Q�[�����̕`��
	/// </summary>
	/// <param name="">�J�E���g�_�E���A�J�E���g�A�b�v�ȊO�̕`��</param>
	void DrawGame(void);

	/// <summary>
	/// �^�C���A�b�v���̕`�揈��
	/// </summary>
	/// <param name=""></param>
	void DrawFinish(void);

	/// <summary>
	/// �X�R�A���摜�ŕ`�悷��
	/// </summary>
	/// <param name="score">�X�R�A</param>
	void DrawScore(int score);
};
