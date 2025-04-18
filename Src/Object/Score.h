//#pragma once
#include<vector>

class Score
{
public:

	static constexpr int RANKING_NUM = 5;

	// �C���X�^���X�̐���
	static void CreateInstance(void);

	// �C���X�^���X�̎擾
	static Score& GetInstance(void);

	void Init(void);
	void Update(void);
	void Draw(void);

	// ���\�[�X�̔j��
	void Destroy(void);

	//
	void GetScore(void);

	//�Q�[���I�����̃X�R�A�ۑ��p
	void SaveScore(int score);

	//�S�X�R�A�����Z�������ʂ��擾
	void GetAggregateScore(void);

	//�����L���O���̃X�R�A���擾
	void GetRankingScore(void);

	void ScoreSort(void);

private:
	// �V���O���g���p�C���X�^���X
	static Score* instance_;

	//�X�R�A�p�̊O���t�@�C���֘A
	FILE* file;		//�t�@�C���\����
	errno_t err;	//�G���[�p

	int score_;
	int addScore_;
	int scoreRank_[5];
	int scoreSort_[5];
};

