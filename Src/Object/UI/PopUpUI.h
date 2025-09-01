#pragma once
#include <string>
#include "UIBase.h"

class PopUpUI : public UIBase
{
public:
    static constexpr float ANIM_DURATION = 1.0f; // �A�j���[�V�������ԁi�b�j
    static constexpr float MOVE_DISTANCE = 1.0f; // ��Ɉړ����鋗���i�s�N�Z���j

    PopUpUI(int score, const VECTOR& startPos);
    virtual ~PopUpUI() = default;

    void Init(void) override;
    void Update(void) override;
    void Draw(void) override;

private:
    int score_;
    float animTime_;

    // �X�R�A�̐����摜�n���h��
    int* numbersImgs_;

    // �`��p�̃��\�b�h
    void DrawScore(int score, const VECTOR& pos, float alpha);
};

