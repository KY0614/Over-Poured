#pragma once
#include <DxLib.h>

class UIBase
{
public:
    //�R���X�g���N�^
    UIBase(void);
    //�f�X�g���N�^
    virtual ~UIBase() = default;

    //����������
    virtual void Init(void) = 0;
    //�X�V����
    virtual void Update(void) = 0;
    //�`�揈��
    virtual void Draw(void) = 0;

    /// <summary>
    /// ���W��ݒ�
    /// </summary>
    /// <param name="pos">�\��������W</param>
    void SetPos(const VECTOR pos) { pos_ = pos; }

	/// <summary>
	/// ���ߓx(���l)��ݒ�
	/// </summary>
	/// <param name="alpha">���ߓx</param>
	void SetAlpha(const float alpha) { alpha_ = alpha; }

    /// <summary>
    /// �\�����邩�ǂ���
    /// </summary>
    /// <param name="isActive">true:�\���@false:��\��</param>
    void SetActive(const bool isActive) { isActive_ = isActive; }

protected:

    //UI�摜�n���h��
    int uiImg_;

    //UI���W
    VECTOR pos_;

    //UI�摜�̓��ߓx
    float alpha_;

	//�\���A��\���p�̃t���O
    bool isActive_;
};

