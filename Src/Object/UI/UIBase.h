#pragma once
#include <DxLib.h>

class UIBase
{
public:

    UIBase(void);
    virtual ~UIBase() = default;

    virtual void Init(void) = 0;
    virtual void Update(void) = 0;
    virtual void Draw() = 0;
    virtual bool IsActive() const = 0;

protected:

    VECTOR pos_;

    float alpha_;

	//表示、非表示用のフラグ
    bool isActive_;
};

