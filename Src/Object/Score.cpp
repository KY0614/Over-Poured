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
	std::ifstream ifs;
	ifs.open(Application::PATH_SCORE + "score.json");
	if (!ifs)
	{
		// �O���t�@�C���̓ǂݍ��ݎ��s
		return;
	}

	// �t�@�C���X�g���[������json�I�u�W�F�N�g�ɕϊ�
	json scoreData = json::parse(ifs);
	//json�I�u�W�F�N�g����Ascore�I�u�W�F�N�g���擾
	const auto& scoreDatas = scoreData["Scores"];
	// enemy�I�u�W�F�N�g�͕�������͂��Ȃ̂ŁA�J��Ԃ�����
	for (const json& scoreData : scoreDatas)
	{
		// �Ǘ��z��ɒǉ�
		scores_.emplace_back(currentScore_);
	}

	//�X�R�[�v���؂��ۂ� �����I�Ƀt�@�C���N���[�Y���ĖႦ�܂����A
	//�����I�Ƀt�@�C���X�g���[�������
	ifs.close();
}

void Score::SaveScore(int score)
{
	//�O���t�@�C���̓ǂݍ���(�����ݗp)
	std::ofstream ofs;
	ofs.open(Application::PATH_SCORE + "score.json");

	scores_.push_back(score);
	int aggScore = 0;
	aggScore = GetAggregateScore() + score;

	//json�I�u�W�F�N�g
	json scoreData;
	//�X�R�A
	scoreData["CurrentScore"].push_back(score);
	scoreData["AggregateScore"].push_back(aggScore);

	//json�I�u�W�F�N�g(�S��)
	json saveData;
	saveData["ScoreData"] = scoreData;
	
	//json�I�u�W�F�N�g��string�ɕϊ�
	auto dmp = saveData.dump();
	
	//�O���t�@�C���ɏ�����
	ofs.write(dmp.c_str(), dmp.size());
	
	//�ϊ��Ə����݂��ȉ��̃��_�C���N�g�ŏȗ���
	//ofs << saveData << std::endl;
	// �X�R�[�v���؂��ۂ� �����I�Ƀt�@�C���N���[�Y���ĖႦ�܂����A
	// ���s�V�ǂ��A�����I�Ƀt�@�C���X�g���[�������
	ofs.close();
}

int Score::GetAggregateScore(void) const
{
	std::ifstream ifs;
	ifs.open(Application::PATH_SCORE + "score.json");
	if (!ifs)
	{
		// �O���t�@�C���̓ǂݍ��ݎ��s
		return -1;
	}

	// �t�@�C���X�g���[������json�I�u�W�F�N�g�ɕϊ�
	json scoreData = json::parse(ifs);

	//json�I�u�W�F�N�g����Ascore�I�u�W�F�N�g���擾
	const auto& scoreDatas = scoreData["AggregateScore"];

	ifs.close();

	return 0;
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
}
