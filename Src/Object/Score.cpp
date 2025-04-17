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

void Score::SaveScore(int score)
{
	score_ = score;
	addScore_ += score;
	//ファイルを開く
	//FILE* fp = fopen("score.dat", "wb");
	std::ofstream file("score.dat", std::ios::app);

	//開けなかったら終了
	//if (fp == NULL) return;
	if (file.is_open()) {
		file << score_ << std::endl;
		file.close();
	}

	//ファイルにスコアの書き出し
	//fwrite(&score_, sizeof(int), 1, fp);

	// ファイルを閉じる
//	fclose(fp);
}

std::vector<int> Score::LoadScore(void)
{
	std::ifstream file("score.dat");
	if (file.is_open()) {
		int score;
		while (file >> score) {
			scores.push_back(score);
		}
		file.close();
	}
	return scores;
}

std::vector<int> Score::GetTopScores(const std::vector<int>& score)
{
	size_t topN = 5;
	std::vector<int> sortedScores = score;
	std::sort(sortedScores.begin(), sortedScores.end(), std::greater<int>());
	if (sortedScores.size() > topN) {
		sortedScores.resize(topN); // 上位N件に制限
	}
	return sortedScores;
}
