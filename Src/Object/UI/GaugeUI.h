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
	int shadowImg_;		// ‰e‚Ì‰æ‘œID
	int circleImg_;		// ‰~Œ`‚Ì‰æ‘œID
	int cShadowImg_;	// ‰~Œ`‚Ì‰e‰æ‘œID

    float activeTime_;  //Å‘å•\¦ŠÔ
    float currentTime_; // Œo‰ßŠÔ
    bool isCircle_;     // true: ‰~Œ`, false: ’·•ûŒ`

	void DrawCircleGauge(float progress);
	void DrawRectGauge(float progress);
};

