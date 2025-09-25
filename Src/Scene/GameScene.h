#pragma once
#include <memory>
#include "SceneBase.h"

class StageManager;
class Player;
class OrderCustomerManager;
class Timer;

class GameScene : public SceneBase
{
public:

	//�R���X�g���N�^
	GameScene(void);

	//�f�X�g���N�^
	~GameScene(void);

	/// <summary>
	/// ����������
	/// </summary>
	void Init(void) override;

	/// <summary>
	/// �X�V����
	/// </summary>
	void Update(void) override;

	/// <summary>
	/// �`�揈��
	/// </summary>
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

	//�֐��|�C���^�ϐ�
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

	/// <summary>
	/// �摜�̓ǂݍ��ݏ���
	/// </summary>
	void LoadImages(void);

	/// <summary>
	/// �T�E���h�̏���������
	/// </summary>
	void InitSound(void);
};
