#pragma once
#include "../Manager/GameSystem/ScoreManager.h"
#include "../Common/Vector2.h"

class Score
{
public:
	//�X�R�A���Z�X�s�[�h
	static constexpr int ADD_SCORE_SPEED = 8;

	//�C�[�W���O�֘A
	static constexpr float START_SLIDE_X = -300.0f;	//�J�n�ʒu
	static constexpr float END_SLIDE_X = 100.0f;		//�I���ʒu
	static constexpr float NEXT_SLIDE_START_X = -125.0f;		//���̃C�[�W���O���J�n����ڕW�ʒu
	static constexpr float SLIDE_MAX_TIME = 1.0f;	//�ڕW����

	static constexpr int RANK_NUM = 4;
	static constexpr int RANK_C_MAX = 499;
	static constexpr int RANK_B_MAX = 999;
	static constexpr int RANK_A_MAX = 1499;
	static constexpr int RANK_S_MAX = 1999;
	
	struct RankInfo {
		int startVal_ = 0, endVal_ = 0;	//�����N���Ƃ̏�������l
		float currentRate_ = 0.0f;			//
		float displayedRate_ = 0.0f;		//
		int gaugeImg_ = -1;	//�Q�[�W�摜	
		Vector2 pos_ = {0,0};	//�\�����W
	};

	enum class RANK {
		C,
		B,
		A,
		S,
		MAX
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

	std::map<STATE, std::function<void(void)>> stateChange_;

	std::function<void(void)> stateUpdate_;
	std::function<void(void)> stateDraw_;

	//����̃Q�[���̃X�R�A
	int currentScr_;

	//�����L���O�X�R�A�����炩�ɕ\��������悤
	float slideX_[ScoreManager::RANKING_NUM];		//�ړ���������W
	float slideTime_[ScoreManager::RANKING_NUM];	//�o�ߎ���
	bool isMove_[ScoreManager::RANKING_NUM];		//�ړ������ǂ����i�f�B���C�������ĕ\������p�j
	
	//�n�C���C�g�_�ŕ\���p�C���f�b�N�X
	int highLightIdx_;	
	//�n�C���C�g�_�ł��J��Ԃ��p�̎���
	float blinkTime_;

	//�X�R�A�ɂ�郉���N�\���p
	RankInfo rankData_[RANK_NUM];

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

