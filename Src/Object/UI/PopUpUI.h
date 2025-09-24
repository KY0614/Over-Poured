#pragma once
#include <string>
#include "UIBase.h"

class PopUpUI : public UIBase
{
public:

    //�R���X�g���N�^
    PopUpUI(int score, const VECTOR& startPos);
    //�f�X�g���N�^
    virtual ~PopUpUI() = default;

    /// <summary>
    /// ����������
    /// </summary>
    void Init(void) override;

    /// <summary>
    /// �X�V����
    /// </summary>
    void Update(void) override;

    /// <summary>
    /// �`�揈��
    /// </summary>
    void Draw(void) override;

private:
    //�X�R�A
    int score_;

	//�A�j���[�V�������Ԍo�ߗp
    float animTime_;

    //�X�R�A�̐����摜�n���h��
    int* numbersImgs_;

    /// <summary>
    /// �X�R�A���t�F�[�h�A�E�g���Ȃ���`�悷��
    /// </summary>
    /// <param name="score">�X�R�A</param>
    /// <param name="pos">�`����W</param>
    /// <param name="alpha">�����x</param>
    void DrawScore(int score, const VECTOR& pos, float alpha);
};

