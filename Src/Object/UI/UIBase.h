#pragma once
#include <DxLib.h>

class UIBase
{
public:

    UIBase(void);
    virtual ~UIBase() = default;

    virtual void Init(void) = 0;
    virtual void Update(void) = 0;
    virtual void Draw(void) = 0;
    virtual bool IsActive() const = 0;

protected:
    //UI�摜�n���h��
    int uiImg_;

    //UI���W
    VECTOR pos_;

    //UI�摜�̓����x
    float alpha_;

	//�\���A��\���p�̃t���O
    bool isActive_;
};

