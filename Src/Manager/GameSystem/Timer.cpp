#include "../Generic/ResourceManager.h"
#include "Timer.h"

Timer::Timer(void)
{
	minute_ = 2;
	second_ = 0;
	cnt_ = 0;
	isEnd_ = false;

	std::string time = "écÇË" + std::to_string(minute_) + ":" + std::to_string(second_);
	int len = (int)strlen(time.c_str());

	pos_.x = Application::SCREEN_SIZE_X / 2.0f;
	pos_.y = 0.0f;
	pos_.z = 0.0f;

	imgNumbers_ = ResourceManager::GetInstance().Load(ResourceManager::SRC::TIMER_NUMBER).handleIds_;
	imgTimerBack_ = ResourceManager::GetInstance().Load(ResourceManager::SRC::UI_BAR).handleId_;
}

Timer::~Timer(void)
{
}

void Timer::Update()
{
	cnt_++;
	//FPSéQè∆ÇÃÉJÉEÉìÉgçXêV
	//FPSÇ∆ìØÇ∂ílÇÃÇ∆Ç´ÇÕàÍïbóßÇ¡ÇΩÇ∆Ç¢Ç§Ç±Ç∆
	if (cnt_ >= 60) {
		cnt_ = 0;

		//éûä‘êßå¿Çå∏ÇÁÇ∑
		ReduceTime();
	}
}

void Timer::Draw()
{
	////îwåiï`âÊ
	//DrawRotaGraph(
	//	Application::SCREEN_SIZE_X / 2,
	//	TIMER_BACK_SIZE_Y / 2,
	//	TIMER_BACK_RATE,
	//	0.0f,
	//	imgTimerBack_,
	//	true);

	int num[NUM_CNT] = { second_ / 10 % 10, second_ % 10 };

	//ïbêîÇÃï`âÊ
	for (int i = 0; i < NUM_CNT; i++) {
		DrawRotaGraph(
			NUM_POS_X[i],
			NUM_POS_Y,
			NUM_RATE,
			0.0f,
			imgNumbers_[num[i]],
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
	//ïbêîÇå∏ÇÁÇ∑
	second_--;

	//ïbêîÇ™ÇOñ¢ñûÇ…Ç»Ç¡ÇΩéû
	if (second_ < TIME_MIN) {
		//ï™êîÇå∏ÇÁÇ∑
		minute_--;
		//ïbêîÇÇTÇXïbÇ…
		second_ = TIME_MAX;
	}

	//ï™êîÇ™ÇOñ¢ñûÇÃÇ∆Ç´èIóπèÛë‘Ç…
	if (minute_ <= TIME_MIN && second_ <= TIME_MIN)
	{
		isEnd_ = true;
	}
}