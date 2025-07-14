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
    };

    OrderUI(Order::DRINK drink,Order::SWEETS sweets);
    virtual ~OrderUI() = default;

    void Init(void) override;
    void Update(void) override;
    void Draw(void) override;

private:
    OrderUIData orderUIData_;

    float size_;
};

