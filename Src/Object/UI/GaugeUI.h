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

	void SetPos(VECTOR pos);
    void SetActiveTime(float time);
    void SetIsCircle(bool flag); // 円形か矩形か
    void Reset(void);

protected:

    float activeTime_;  //最大表示時間
    float currentTime_; // 経過時間
    bool isCircle_;     // true: 円形, false: 長方形
};

