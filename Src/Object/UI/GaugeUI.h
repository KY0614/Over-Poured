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
    void SetIsCircle(bool flag); // �~�`����`��
    void Reset(void);

protected:

    float activeTime_;  //�ő�\������
    float currentTime_; // �o�ߎ���
    bool isCircle_;     // true: �~�`, false: �����`
};

