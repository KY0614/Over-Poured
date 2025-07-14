#pragma once
#include <DxLib.h>
#include "../../Application.h"

class Timer
{
public:
	//���Ԃ̏���E����
	static constexpr int TIME_MAX = 59;
	static constexpr int TIME_MIN = 0;

	//�^�C�}�[�w�i�T�C�Y
	static constexpr float TIMER_BACK_RATE = 0.8f;
	static constexpr int TIMER_BACK_SIZE_X = 298 * TIMER_BACK_RATE;
	static constexpr int TIMER_BACK_SIZE_Y = 168 * TIMER_BACK_RATE;

	//�����̊Ԋu
	static constexpr int MARGIN_SIZE = 50;
	static constexpr int MARGIN_SIZE_COLON = 10; // �R���������̃}�[�W��

	//�����傫��
	static constexpr float NUM_RATE = 0.75f;

	//�����`�搔
	static constexpr int NUM_CNT = 4;

	//�����̕`��ʒu
	static constexpr int NUM_POS_X[NUM_CNT] = {
		Application::SCREEN_SIZE_X / 2 - (MARGIN_SIZE * 2) + MARGIN_SIZE_COLON,
		Application::SCREEN_SIZE_X / 2 - MARGIN_SIZE + MARGIN_SIZE_COLON,
		Application::SCREEN_SIZE_X / 2 + MARGIN_SIZE,
		Application::SCREEN_SIZE_X / 2 + (MARGIN_SIZE * 2),
	};
	static constexpr int NUM_POS_Y = 40;

	Timer(int minute,int second);
	~Timer(void);

	void Update();
	void Draw();
	void Reset();

	bool IsEnd(void) const { return isEnd_; }

private:
	int minute_;	//����
	int second_;	//�b��

	bool isEnd_;	//�^�C�}�[�I���̒m�点

	int cnt_;	//�J�E���^�[

	VECTOR pos_;	//�ʒu

	int imgTimerBack_;	//�w�i
	int colonImg_;		//�R�����i�F�j
	int* NumberImgs_;	//�����摜

	void ReduceTime(void);
};

