//#pragma once

#include<vector>

class ScoreManager
{
public:

	static constexpr int RANKING_NUM = 5;

	//�C���X�^���X�̐���
	static void CreateInstance(void);

	//�C���X�^���X�̎擾
	static ScoreManager& GetInstance(void);

	void Init(void);

	//���\�[�X�̔j��
	void Destroy(void);

	//
	void LoadScore(void);

	//�Q�[���I�����̃X�R�A�ۑ��p
	void SaveScore(int score);

	int GetCurrentScore(void)const { return currentScore_; }

	//�S�X�R�A�����Z�������ʂ��擾
	int GetAggregateScore(void)const;

	//�����L���O���̃X�R�A���擾
	int GetRankingScore(int element)const { return scoreRank_[element]; }

	void SetCurrentScore(int score) { currentScore_ = score; }

private:

	//�V���O���g���p�C���X�^���X
	static ScoreManager* instance_;

	//�X�R�A	
	std::vector<int> scores_;
	int currentScore_;		//����̃X�R�A
	int scoreRank_[6];	//�����L���O�`���̃X�R�A

	void UpdateRanking(void);
};

