//#pragma once

#include<vector>

class Score
{
public:

	static constexpr int RANKING_NUM = 5;

	//�C���X�^���X�̐���
	static void CreateInstance(void);

	//�C���X�^���X�̎擾
	static Score& GetInstance(void);

	void Init(void);

	//���\�[�X�̔j��
	void Destroy(void);

	//
	void LoadScore(void);
	//void LoadCurrentScore(void);
	//void LoadAggScore(void);

	//�Q�[���I�����̃X�R�A�ۑ��p
	void SaveScore(int score);

	int GetCurrentScore(void)const { return currentScore_; }

	//�S�X�R�A�����Z�������ʂ��擾
	int GetAggregateScore(void)const;

	//�����L���O���̃X�R�A���擾
	//const std::vector<int>& GetRankingScore(void)const { return rankingScores_; }

	void SetCurrentScore(int score) { currentScore_ = score; }

	void AddScore(int score) { addScore_ += score; }

private:

	//�V���O���g���p�C���X�^���X
	static Score* instance_;

	//�X�R�A	
	std::vector<int> scores_;
	int currentScore_;		//����̃X�R�A
	int addScore_;	//�S�ẴX�R�A�����Z�����X�R�A
	int scoreRank_[6];	//�����L���O�`���̃X�R�A
	int scoreSort_[6];	//�\�[�g�p�̃X�R�A

	void UpdateRanking(void);
};

