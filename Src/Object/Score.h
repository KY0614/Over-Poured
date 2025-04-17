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

	void SaveScore(int score);
	std::vector<int> LoadScore(void);
	std::vector<int> GetTopScores(const std::vector<int>& score);
	int GetTotalScore(void);
private:
	// �V���O���g���p�C���X�^���X
	static Score* instance_;

	int score_;
	int addScore_;
	int scoreRank_[5];
	std::vector<int> scores;
};

