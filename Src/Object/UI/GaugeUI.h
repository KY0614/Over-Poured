#pragma once
#include "UIBase.h"

class GaugeUI : public UIBase
{
public:

    GaugeUI(bool isCircle, float activeTime);

	~GaugeUI(void) = default;

	void Init(void) override;
    void Update(void) override;
    void Draw(void) override;

	void SetActiveTime(const float time) { activeTime_ = time; }
	void SetIsCircle(const bool flag) { isCircle_ = flag; }
    void Reset(void);

private:
	int shadowImg_; // 影の画像ID
	int circleImg_; // 円形の画像ID

    float activeTime_;  //最大表示時間
    float currentTime_; // 経過時間
    bool isCircle_;     // true: 円形, false: 長方形

	void DrawCircleGauge(float progress);
	void DrawRectGauge(float progress);
};

