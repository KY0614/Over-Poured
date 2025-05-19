#include <DxLib.h>
#include <fstream>
#include"../Libs/nlohmann/json.hpp"
#include"../Application.h"
#include "Score.h"

Score* Score::instance_ = nullptr;

// �����̂�namespace�̏ȗ�
using json = nlohmann::json;

void Score::CreateInstance(void)
{
	if (instance_ == nullptr)
	{
		instance_ = new Score();
	}
	instance_->Init();
}

Score& Score::GetInstance(void)
{
	if (instance_ == nullptr)
	{
		Score::CreateInstance();
	}
	return *instance_;
}

void Score::Init(void)
{
}

void Score::Destroy(void)
{
	delete instance_;
}

void Score::LoadScore(void)
{
	scores_.clear();
	std::ifstream ifs(Application::PATH_SCORE + "score.json");
	if (!ifs)return;//�ǂݍ��ݎ��s

	// �t�@�C���X�g���[������json�I�u�W�F�N�g�ɕϊ�
	json scoreData = json::parse(ifs);

	//json�I�u�W�F�N�g����Ascore�I�u�W�F�N�g���擾
	const auto& scoreDatas = scoreData["Scores"];
	// score�I�u�W�F�N�g�͕�������͂��Ȃ̂ŁA�J��Ԃ�����
	for (const json& s : scoreDatas)
	{
		// �Ǘ��z��ɒǉ�
		scores_.emplace_back(s.get<int>());
	}

	//�X�R�[�v���؂��ۂ� �����I�Ƀt�@�C���N���[�Y���ĖႦ�܂����A
	//�����I�Ƀt�@�C���X�g���[�������
	ifs.close();

	UpdateRanking(); //�����L���O���X�V
}

void Score::SaveScore(int score)
{
	std::ifstream ifs(Application::PATH_SCORE + "score.json");
	json scoreData;
	if (ifs)
	{
		scoreData = json::parse(ifs);
		ifs.close();
	}

	if (!scoreData.contains("Scores")) {
		scoreData["Scores"] = json::array();
	}

	scoreData["Scores"].push_back(score);

	std::ofstream ofs(Application::PATH_SCORE + "score.json");
	ofs << scoreData.dump(4);
	ofs.close();
}

int Score::GetAggregateScore(void) const
{
	int total = 0;
	for (int s : scores_) {
		total += s;
	}
	return total;
}

void Score::UpdateRanking(void)
{
	//�X�R�A���\�[�g����
	std::vector<int> scoreList = scores_;
	std::sort(scoreList.begin(), scoreList.end(), std::greater<int>());

	//�T�ʂ܂ł̃X�R�A���~���œ����(�X�R�A���T�ȏ�Ȃ��ꍇ�͂O������)
	for (int i = 0; i < RANKING_NUM && i < scoreList.size(); ++i) {
		scoreRank_[i] = scoreList[i];
	}
	for (int i = scoreList.size(); i < RANKING_NUM; ++i) {
		scoreRank_[i] = 0;
	}
}
