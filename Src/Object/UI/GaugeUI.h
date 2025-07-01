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
	int shadowImg_;		// �e�̉摜ID
	int circleImg_;		// �~�`�̉摜ID
	int cShadowImg_;	// �~�`�̉e�摜ID

    float activeTime_;  //�ő�\������
    float currentTime_; // �o�ߎ���
    bool isCircle_;     // true: �~�`, false: �����`

	void DrawCircleGauge(float progress);
	void DrawRectGauge(float progress);
};

