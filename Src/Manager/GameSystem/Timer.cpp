#include "../Generic/ResourceManager.h"
#include "Timer.h"

Timer::Timer(int minute, int second)
{
	//ƒ^ƒCƒ}[‚ÌŠÔ‚Í
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
	//FPSQÆ‚ÌƒJƒEƒ“ƒgXV
	//FPS‚Æ“¯‚¶’l‚Ì‚Æ‚«‚Íˆê•b—§‚Á‚½‚Æ‚¢‚¤‚±‚Æ(60‚Å‚P•bj
	if (cnt_ >= TIME_MAX + 1) {
		cnt_ = 0;

		//ŠÔ§ŒÀ‚ğŒ¸‚ç‚·
		ReduceTime();
	}
}

void Timer::Draw()
{
	////”wŒi•`‰æ
	//DrawRotaGraph(
	//	Application::SCREEN_SIZE_X / 2,
	//	TIMER_BACK_SIZE_Y / 2,
	//	TIMER_BACK_RATE,
	//	0.0f,
	//	imgTimerBack_,
	//	true);

	int num[NUM_CNT] = {
		minute_ / 10,       // •ª‚Ì\‚ÌˆÊ
		minute_ % 10,       // •ª‚Ìˆê‚ÌˆÊ
		second_ / 10,       // •b‚Ì\‚ÌˆÊ
		second_ % 10        // •b‚Ìˆê‚ÌˆÊ
	};

	DrawRotaGraph(
		Application::SCREEN_SIZE_X / 2,
		NUM_POS_Y,
		1.0f, 0.0f,
		colonImg_,
		true, false
	);

	//•b”‚Ì•`‰æ
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
	//•b”‚ğŒ¸‚ç‚·
	second_--;

	//•b”‚ª‚O–¢–‚É‚È‚Á‚½
	if (second_ < TIME_MIN) {
		//•ª”‚ğŒ¸‚ç‚·
		minute_--;
		//•b”‚ğ‚T‚X•b‚É
		second_ = TIME_MAX;
	}

	//•ª”‚ª‚O–¢–‚Ì‚Æ‚«I—¹ó‘Ô‚É
	if (minute_ <= TIME_MIN && second_ <= TIME_MIN)
	{
		isEnd_ = true;
	}
}