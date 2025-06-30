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

    //void SetProgress(float rate); // 0.0f�`1.0f
	void SetPos(VECTOR pos);
    void SetActiveTime(float time);
    void SetIsCircle(bool flag); // �~�`����`��
    void Reset(void);

protected:
    float progress_;    //���݂̐i��
    float activeTime_;  //�\������
    float currentTime_; // �o�ߎ���
    bool isCircle_;     // true: �~�`, false: �����`

    int gaugeImg_; // ���ʉ摜
};

