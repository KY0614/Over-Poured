#pragma once
#include "../Manager/GameSystem/ScoreManager.h"
#include "../Common/Vector2.h"

class Score
{
public:
	//�����N�̐�
	static constexpr int RANK_NUM = 4;		

	struct RankInfo 
	{
		int startVal_ = 0, endVal_ = 0;	//�����N���Ƃ̏�������l
		float currentRate_ = 0.0f;		//
		float displayedRate_ = 0.0f;	//
		int gaugeImg_ = -1;		//�Q�[�W�摜	
		bool isFull_ = false;
	};

	//�����N�̎��
	enum class RANK 
	{
		C,
		B,
		A,
		S,
		MAX
	};

	//�X�R�A��ʂ̏��
	enum class STATE
	{
		PLAY_SCORE,		//����̃X�R�A�ƃ����L���O�\��
		TOTAL_SCORE		//���X�R�A�\��
	};

	// �R���X�g���N�^
	Score(void);

	// �f�X�g���N�^
	~Score(void);

	/// <summary>
	/// ����������
	/// </summary>
	void Init(void);

	/// <summary>
	/// �X�V����
	/// </summary>
	void Update(void);

	/// <summary>
	/// �`�揈��
	/// </summary>
	void Draw(void);

private:
	//�X�R�A��ʂ̏��
	STATE state_;

	//����̃Q�[���̃����N
	RANK rank_;
	
	//��ԊǗ�(��ԑJ�ڎ���������)
	std::map<STATE, std::function<void(void)>> stateChange_;
	//��ԊǗ�(��ԍX�V����)
	std::function<void(void)> stateUpdate_;
	//��ԊǗ�(��ԕ`�揈��)
	std::function<void(void)> stateDraw_;

	//���݃X�R�A��`��ς݂��ǂ���
	bool isCurrentScrDraw_;	//true:�`��ς� false:���`��

	//�����L���O�X�R�A��`��ς݂��ǂ���
	bool isRankingScrDraw_;	//true:�`��ς� false:���`��

	//�Q�[�W��`��ς݂��ǂ���
	bool isGaugeDraw_;		//true:�`��ς� false:���`��

	//����̃Q�[���̃X�R�A
	int currentScr_;

	//�����L���O�X�R�A�����炩�ɕ\��������悤
	float slideX_[ScoreManager::RANKING_NUM];		//�ړ���������W
	float slideXTime_[ScoreManager::RANKING_NUM];	//�o�ߎ���
	bool isRankScrMove_[ScoreManager::RANKING_NUM];	//�ړ������ǂ����i�f�B���C�������ĕ\������p�j
	
	//�n�C���C�g�_�ŕ\���p�C���f�b�N�X
	int highLightIdx_;	
	//�n�C���C�g�_�ł��J��Ԃ��p�̎���
	float blinkTime_;

	//�X�R�A�ɂ�郉���N�\���p
	RankInfo rankData_[RANK_NUM];

	//�Q�[�W��i�܂���p�̎���
	float gaugeTime_;

	//���݃Q�[�W��i�܂��Ă��郉���N�̃C���f�b�N�X
	int currentRankIdx_;

	//�����L���O�̃��x���摜�i�P�ʁF�`�T�ʁF�j
	int* rankLabelImgs_;
	//�����L���O�̔w�i�摜
	int rankingBackImg_;

	//�����p
	int* numberImgs_;
	//�u���݃X�R�A�v���x���摜
	int currentScrImg_;
	//�����N���Ƃ̕����摜�iC,B,A,S)
	int* ranksImgs_;
	//�Q�[�W�摜
	int circleShadowImg_;
	//�w�i�̑����摜
	int decoImg_;
	//push space or Button�摜
	int pushImg_;	
	
	//��ʔ䗦
	float aspectRatio_;

	/// <summary>
	/// ���݃X�R�A���O���瑫���Ȃ���`��
	/// </summary>
	/// <param name="score">���݃X�R�A</param>
	/// <param name="posX">X���W</param>
	/// <param name="posY">Y���W</param>
	/// <param name="scale">1����������̑傫��</param>
	void DrawVariableScore(int score,int posX,int posY, float scale);

	/// <summary>
	/// �����L���O�X�R�A��`��
	/// </summary>
	/// <param name="score">���݃X�R�A</param>
	/// <param name="posX">X</param>
	/// <param name="posY">Y���W</param>
	/// <param name="hightLight">�n�C���C�g</param>
	void DrawRankingScore(int score,int posX,int posY,int hightLight);

	/// <summary>
	/// ��Ԃ�ύX
	/// </summary>
	/// <param name="state">�ύX������</param>
	void ChangeState(STATE state);

	/// <summary>
	/// PLAYSCORE��ԂɕύX
	/// </summary>
	/// <param name=""></param>
	void ChangePlayScore(void);

	/// <summary>
	/// PLAYSCORE��Ԃ̍X�V
	/// </summary>
	/// <param name=""></param>
	void UpdatePlayScore(void);

	/// <summary>
	/// PLAYSCORE��Ԃ̕`��
	/// </summary>
	/// <param name=""></param>
	void DrawPlayScore(void);

	/// <summary>
	/// �����N���Ƃ̃p�[�Z���g���v�Z
	/// </summary>
	/// <param name="">�Q�[�W�̃p�[�Z���e�[�W���C�[�W���O���g���ĉ��Z���Ă���</param>
	void CalcPercentFromRank(void);

	/// <summary>
	/// �X�R�A���烉���N���擾
	/// </summary>
	/// <param name="score">�X�R�A</param>
	/// <returns>�X�R�A�ɂ�郉���N</returns>
	RANK GetRankFromScore(const int score);

	/// <summary>
	/// �����N���̏�����
	/// </summary>
	void InitRankInfo(void);

	/// <summary>
	/// �摜�̓ǂݍ���
	/// </summary>
	void LoadImages(void);

	/// <summary>
	/// ���̓ǂݍ���
	/// </summary>
	void InitSound(void);
};

