#include "../Generic/ResourceManager.h"
#include "Timer.h"

Timer::Timer(int minute, int second)
{
	//�^�C�}�[�̎��Ԃ�
	minute_ = minute;
	second_ = second;
	cnt_ = 0;
	isEnd_ = false;

	pos_.x = (float)Application::SCREEN_SIZE_X / 2;
	pos_.y = 0.0f;
	pos_.z = 0.0f;

	NumberImgs_ = ResourceManager::GetInstance().Load(ResourceManager::SRC::TIMER_NUMBER).handleIds_;
	colonImg_ = ResourceManager::GetInstance().Load(ResourceManager::SRC::TIMER_COLON).handleId_;
	imgTimerBack_ = ResourceManager::GetInstance().Load(ResourceManager::SRC::UI_BAR).handleId_;
}

Timer::~Timer(void)
{
}

void Timer::Update()
{
	cnt_++;
	//FPS�Q�Ƃ̃J�E���g�X�V
	//FPS�Ɠ����l�̂Ƃ��͈�b�������Ƃ�������(60�łP�b�j
	if (cnt_ >= TIME_MAX + 1) {
		cnt_ = 0;

		//���Ԑ��������炷
		ReduceTime();
	}
}

void Timer::Draw()
{
	////�w�i�`��
	//DrawRotaGraph(
	//	Application::SCREEN_SIZE_X / 2,
	//	TIMER_BACK_SIZE_Y / 2,
	//	TIMER_BACK_RATE,
	//	0.0f,
	//	imgTimerBack_,
	//	true);

	int num[NUM_CNT] = {
		minute_ / 10,       // ���̏\�̈�
		minute_ % 10,       // ���̈�̈�
		second_ / 10,       // �b�̏\�̈�
		second_ % 10        // �b�̈�̈�
	};

	DrawRotaGraph(
		Application::SCREEN_SIZE_X / 2,
		NUM_POS_Y,
		1.0f, 0.0f,
		colonImg_,
		true, false
	);

	//�b���̕`��
	for (int i = 0; i < NUM_CNT; i++) {
		DrawRotaGraph(
			NUM_POS_X[i],
			NUM_POS_Y,
			NUM_RATE,
			0.0f,
			NumberImgs_[num[i]],
			true);
	}
}

void Timer::Reset()
{
	minute_ = 1;
	second_ = 0;
	cnt_ = 0;
	isEnd_ = false;
}

void Timer::ReduceTime(void)
{
	//�b�������炷
	second_--;

	//�b�����O�����ɂȂ�����
	if (second_ < TIME_MIN) {
		//���������炷
		minute_--;
		//�b�����T�X�b��
		second_ = TIME_MAX;
	}

	//�������O�����̂Ƃ��I����Ԃ�
	if (minute_ <= TIME_MIN && second_ <= TIME_MIN)
	{
		isEnd_ = true;
	}
}