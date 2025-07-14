#pragma once
#include "UIBase.h"
#include "../Order/Order.h"

class OrderUI :  public UIBase
{
public:

    static constexpr float BACK_IMG_SIZE = 300.0f;

    struct OrderUIData
    {
        Order::DRINK drinkType_;
        Order::SWEETS sweetsType_;

        int drinkUIImg_;
        int sweetsUIImg_;
        int backUIImg_;

        int timerBackImg_;
        int timerImg_;

        float currentRate_ = 0.0f;	
        float displayedRate_ = 0.0f;
    };

    OrderUI(Order::DRINK drink,Order::SWEETS sweets,float maxTime);
    virtual ~OrderUI() = default;

    void Init(void) override;
    void UpdateTimeGauge(float orderTime);
    void Update(void) override;
    void Draw(void) override;

private:

    OrderUIData orderUIData_;

    //ゲージを進ませる用の時間
    float gaugeTime_;

    float orderTime_;

    float size_;
};

