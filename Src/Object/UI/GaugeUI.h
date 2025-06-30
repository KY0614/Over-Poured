#pragma once
#include "UIBase.h"

class GaugeUI : public UIBase
{
public:
    GaugeUI(void);

	~GaugeUI(void) = default;

	void Init(void) override;
    void Update() override;
    void Draw() override;

    //void SetProgress(float rate); // 0.0f～1.0f
	void SetPos(VECTOR pos);
    void SetActiveTime(float time);
    void SetIsCircle(bool flag); // 円形か矩形か
    void Reset(void);

protected:
    float progress_;    //現在の進捗
    float activeTime_;  //表示時間
    float currentTime_; // 経過時間
    bool isCircle_;     // true: 円形, false: 長方形

    int gaugeImg_; // 共通画像
};

