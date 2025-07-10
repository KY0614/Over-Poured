#pragma once
#include "../Manager/GameSystem/ScoreManager.h"
#include "../Common/Vector2.h"

class Score
{
public:
	//�X�R�A���Z�X�s�[�h
	static constexpr int ADD_SCORE_SPEED = 8;
	static constexpr int ADD_CURRENT_SPEED = 5;
	static constexpr int ADD_TOTALSCORE_SPEED = 123;

	//�C�[�W���O�֘A
	static constexpr float START_SLIDE_X = -300.0f;	//X�J�n�ʒu
	static constexpr float END_SLIDE_X = 100.0f;	//X�I���ʒu
	static constexpr float START_SLIDE_Y = 690.0f;	//Y�J�n�ʒu
	static constexpr float END_SLIDE_Y = 420.0f;	//Y�I���ʒu
	static constexpr float NEXT_SLIDE_START_X = -125.0f;	//���̃C�[�W���O���J�n����ڕW�ʒu
	static constexpr float SLIDE_MAX_TIME = 1.0f;	//�ڕW����
	static constexpr float BLINK_SPEED = 0.5f;		//�n�C���C�g�_�ł̊Ԋu

	static constexpr float MAX_GAUGE_TIME = 1.0f;		//�Q�[�W�̃C�[�W���O�ڕW����
	static constexpr float FIRST_GAUGE_SPEED = 0.5f;	//�ŏ��ɕ\������Q�[�W�̃X�s�[�h
	static constexpr float CURRENT_GAUGE_SPEED = 0.5f;	//�Ō�ɕ\������Q�[�W�̃X�s�[�h
	
	//�����N�֘A
	static constexpr int RANK_NUM = 4;		//�����N�̐�
	static constexpr int RANK_C_MAX = 500;	//C�����N�̍ő�l
	static constexpr int RANK_B_MAX = 1000;	//B�����N�̍ő�l
	static constexpr int RANK_A_MAX = 1500;	//A�����N�̍ő�l
	static constexpr int RANK_S_MAX = 1999;	//S�����N�̍ő�l

	static constexpr int RANK_C_MIN = 0;	//C�����N�̍ŏ��l
	static constexpr int RANK_B_MIN = 501;	//B�����N�̍ŏ��l
	static constexpr int RANK_A_MIN = 1001;	//A�����N�̍ŏ��l
	static constexpr int RANK_S_MIN = 1501;	//S�����N�̍ŏ��l
	
	struct RankInfo {
		int startVal_ = 0, endVal_ = 0;	//�����N���Ƃ̏�������l
		float currentRate_ = 0.0f;		//
		float displayedRate_ = 0.0f;	//
		int gaugeImg_ = -1;		//�Q�[�W�摜	
		bool isFull_ = false;
	};

	enum class RANK {
		C,
		B,
		A,
		S,
		MAX
	};

	enum class TOTALSCR_PHASE {
		COUNT_UP,       // �J�E���g�A�b�v��
		SHOW_CURRENT,   // ����̃X�R�A��ʕ\��
		MOVE_TO_TOTAL,  // �ړ���
		MERGE,          // ���̉��o
		FINISH          // �I��
	};

	enum class STATE
	{
		PLAY_SCORE,
		TOTAL_SCORE
	};

	// �R���X�g���N�^
	Score(void);

	// �f�X�g���N�^
	~Score(void);

	void Init(void);
	void Update(void);
	void Draw(void);

private:
	STATE state_;

	RANK rank_;
	TOTALSCR_PHASE phase_;

	std::map<STATE, std::function<void(void)>> stateChange_;

	std::function<void(void)> stateUpdate_;
	std::function<void(void)> stateDraw_;

	//����̃Q�[���̃X�R�A
	int currentScr_;

	//�S�v���C���[�̑��X�R�A
	int totalScr_;

	//�����L���O�X�R�A�����炩�ɕ\��������悤
	float slideX_[ScoreManager::RANKING_NUM];		//�ړ���������W
	float slideXTime_[ScoreManager::RANKING_NUM];	//�o�ߎ���
	bool isMove_[ScoreManager::RANKING_NUM];		//�ړ������ǂ����i�f�B���C�������ĕ\������p�j
	
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

	float slideY_;
	float slideYTime_;

	int circleShadowImg_;

	void DebugDraw(void);

	void ChangeState(STATE state);
	void ChangePlayScore(void);
	void ChangeTotalScore(void);

	void UpdatePlayScore(void);
	void UpdateTotalScore(void);

	void DrawPlayScore(void);
	void DrawTotalScore(void);

	void CalcPercentFromRank(void);

	RANK GetRankFromScore(int score);

	void InitRankInfo(void);

};

