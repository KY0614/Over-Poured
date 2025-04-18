#include <DxLib.h>
#include <string>
#include <fstream>
#include <algorithm>
#include <numeric>
#include <stdio.h>
#include "Score.h"

Score* Score::instance_ = nullptr;

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

void Score::GetScore(void)
{
	err = fopen_s(&file, "Data/score.txt", "r");
	if (err != 0) { //エラー発生
		return;
	}
}

void Score::SaveScore(int score)
{
	err = fopen_s(&file, "Data/score.txt", "w");
	if (err != 0) { //エラー発生
		return;
	}

	fprintf(file, "今回のゲームのスコア ： %d\n", score);
	fclose(file);
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
