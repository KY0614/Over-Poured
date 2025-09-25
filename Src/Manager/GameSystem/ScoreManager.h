//#pragma once

#include<vector>

class ScoreManager
{
public:
	//�����L���O�̐�
	static constexpr int RANKING_NUM = 5;

	//�C���X�^���X�̐���
	static void CreateInstance(void);

	//�C���X�^���X�̎擾
	static ScoreManager& GetInstance(void);

	void Init(void);

	//���\�[�X�̔j��
	void Destroy(void);

	/// <summary>
	/// �X�R�A�̓ǂݍ���
	/// </summary>
	void LoadScore(void);

	/// <summary>
	/// �Q�[���I�����̃X�R�A��json�ɕۑ�
	/// </summary>
	/// <param name="score">�ۑ�����X�R�A</param>
	void SaveScore(const int score);

	/// <summary>
	/// ����̃X�R�A���擾
	/// </summary>
	/// <returns>����̃X�R�A</returns>
	const int& GetCurrentScore(void)const { return currentScore_; }

	/// <summary>
	/// �S�X�R�A�����Z�������ʂ��擾
	/// </summary>
	/// <param name=""></param>
	/// <returns>�S�X�R�A�����Z��������</returns>
	const int& GetAggregateScore(void)const;

	//�����L���O���̃X�R�A���擾

	/// <summary>
	/// �w�肵���v�f�̃����L���O�̃X�R�A���擾���܂��B
	/// </summary>
	/// <param name="element">�����L���O�X�R�A���擾����v�f�̃C���f�b�N�X</param>
	/// <returns>�w�肵���v�f�̃����L���O�X�R�A�ւ̎Q�ƁB</returns>
	const int& GetRankingScore(int element)const { return scoreRank_[element]; }

	/// <summary>
	/// ���݂̃X�R�A��ݒ肵�܂��B
	/// </summary>
	/// <param name="score">�ݒ肷��X�R�A�̒l</param>
	void SetCurrentScore(const int score) { currentScore_ = score; }

private:

	//�V���O���g���p�C���X�^���X
	static ScoreManager* instance_;

	//�X�R�A	
	std::vector<int> scores_;
	int currentScore_;		//����̃X�R�A
	int scoreRank_[6];	//�����L���O�`���̃X�R�A

	/// <summary>
	/// �X�R�A���\�[�g����
	/// </summary>
	void SortRankingScore(void);
};

