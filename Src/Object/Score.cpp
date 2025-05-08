#include <DxLib.h>
#include <string>
#include <fstream>
#include <algorithm>
#include <numeric>
#include <stdio.h>
#include"../Libs/nlohmann/json.hpp"
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

void Score::Update(void)
{
}

void Score::Draw(void)
{
}

void Score::Destroy(void)
{
	delete instance_;
}

void Score::LoadScore(void)
{
	char buffer[256];
	err = fopen_s(&file, "Data/Score/score.txt", "r");
	if (err != 0) { //�G���[����
		return;
	}
	// �t�@�C���̓��e��ǂݍ���ŕ\��
	while (fgets(buffer, sizeof(buffer), file) != NULL) {
		printf("%s", buffer);
	}
	fclose(file);
}

void Score::SaveScore(int score)
{
	err = fopen_s(&file, "Data/Score/score.txt", "w");
	if (err != 0) { //�G���[����
		return;
	}
	addScore_ += score;

	fprintf(file, "����̃Q�[���̃X�R�A �F %d\n", score);

	fclose(file);

	// �O���t�@�C���̓ǂݍ���(�����ݗp)
	std::ofstream ofs;
	ofs.open("Data/Score/score.json");

	// json�I�u�W�F�N�g
	json scoreData;
	//�X�R�A
	scoreData["Score"].push_back(score);
	scoreData["AddScore"].push_back(addScore_);

	// json�I�u�W�F�N�g(�S��)
	json saveData;
	saveData["ScoreData"] = scoreData;
	
	// json�I�u�W�F�N�g��string�ɕϊ�
	auto dmp = saveData.dump();
	
	// �O���t�@�C���ɏ�����
	ofs.write(dmp.c_str(), dmp.size());
	
	// �ϊ��Ə����݂��ȉ��̃��_�C���N�g�ŏȗ���
	//ofs << saveData << std::endl;
	// �X�R�[�v���؂��ۂ� �����I�Ƀt�@�C���N���[�Y���ĖႦ�܂����A
	// ���s�V�ǂ��A�����I�Ƀt�@�C���X�g���[�������
	ofs.close();

}

void Score::GetAggregateScore(void)
{
}

void Score::GetRankingScore(void)
{
}

void Score::ScoreSort(void)
{
}
