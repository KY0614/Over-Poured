#include <DxLib.h>
#include <algorithm>
#include <iostream>
#include <fstream>
#include <stdio.h>
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
	// enemy�I�u�W�F�N�g�͕�������͂��Ȃ̂ŁA�J��Ԃ�����
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
	////�O���t�@�C���̓ǂݍ���(�����ݗp)
	//std::ofstream ofs;
	//ofs.open(Application::PATH_SCORE + "score.json");

	//scores_.push_back(score);
	//int aggScore = 0;
	//aggScore = GetAggregateScore() + score;

	////json�I�u�W�F�N�g
	//json scoreData;
	////�X�R�A
	//scoreData["CurrentScore"].push_back(score);
	//scoreData["AggregateScore"].push_back(aggScore);

	////json�I�u�W�F�N�g(�S��)
	//json saveData;
	//saveData["ScoreData"] = scoreData;
	//
	////json�I�u�W�F�N�g��string�ɕϊ�
	//auto dmp = saveData.dump();
	//
	////�O���t�@�C���ɏ�����
	//ofs.write(dmp.c_str(), dmp.size());
	//
	////�ϊ��Ə����݂��ȉ��̃��_�C���N�g�ŏȗ���
	////ofs << saveData << std::endl;
	//// �X�R�[�v���؂��ۂ� �����I�Ƀt�@�C���N���[�Y���ĖႦ�܂����A
	//// ���s�V�ǂ��A�����I�Ƀt�@�C���X�g���[�������
	//ofs.close();

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
	//// �X�R�A��vector�ɃR�s�[
	//std::vector<int> scoreList;
	//for (const auto& s : scores_) {
	//	scoreList.push_back(s["score"].get<int>());
	//}

	//// �\�[�g
	//std::sort(scoreList.begin(), scoreList.end(), std::greater<int>());

	//// �����L���O���X�V
	//rankingScores_.clear();
	//for (int i = 0; i < /*std::min((int)*/scoreList.size(), RANKING_NUM; ++i) {
	//	rankingScores_.push_back(scoreList[i]);
	//}

	std::vector<int> scoreList = scores_;
	std::sort(scoreList.begin(), scoreList.end(), std::greater<int>());

	for (int i = 0; i < RANKING_NUM && i < scoreList.size(); ++i) {
		scoreRank_[i] = scoreList[i];
	}
	for (int i = scoreList.size(); i < RANKING_NUM; ++i) {
		scoreRank_[i] = 0;
	}
}
