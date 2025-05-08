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
	void Update(void);
	void Draw(void);

	//���\�[�X�̔j��
	void Destroy(void);

	//
	void LoadScore(void);

	//�Q�[���I�����̃X�R�A�ۑ��p
	void SaveScore(int score);

	//�S�X�R�A�����Z�������ʂ��擾
	void GetAggregateScore(void);

	//�����L���O���̃X�R�A���擾
	void GetRankingScore(void);

	void ScoreSort(void);

	void AddScore(int score) { addScore_ += score; }

private:

	//�V���O���g���p�C���X�^���X
	static Score* instance_;

	//�X�R�A�p�̊O���t�@�C���֘A
	FILE* file;		//�t�@�C���\����
	errno_t err;	//�G���[�p

	//�X�R�A	
	int score_;		//����̃X�R�A
	int addScore_;	//�S�ẴX�R�A�����Z�����X�R�A
	int scoreRank_[6];	//�����L���O�`���̃X�R�A
	int scoreSort_[6];	//�\�[�g�p�̃X�R�A
};

