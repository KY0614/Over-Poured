#include "FpsCounter.h"
#include "DxLib.h"

FpsCounter::FpsCounter()
    : prevTime_(GetNowCount()), instantFps_(0.0f)
{
}

void FpsCounter::Update()
{
    int now = GetNowCount();
    int delta = now - prevTime_;
    if (delta > 0)
    {
        instantFps_ = 1000.0f / delta;
    }
    prevTime_ = now;
}

void FpsCounter::Draw(int x, int y)
{
    DrawFormatString(x, y, GetColor(255,255,255), L"FPS: %.1f", instantFps_);
}