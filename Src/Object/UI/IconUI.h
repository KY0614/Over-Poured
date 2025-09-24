#pragma once
#include "../../Manager/Generic/ResourceManager.h"
#include "UIBase.h"

class IconUI : public UIBase
{
public:
    //�R���X�g���N�^
    IconUI(VECTOR localPos,VECTOR parentPos,ResourceManager::SRC src);
	//�f�X�g���N�^
    ~IconUI() = default;

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

    /// <summary>
	/// UI�̃T�C�Y��ݒ�
    /// </summary>
    /// <param name="size"></param>
    void SetUISize(const float size) { size_ = size; }

protected:
	//���[�J�����W
    VECTOR localPos_;
	//�Ǐ]��̍��W
    VECTOR parentPos_;
    //�T�C�Y
    float size_;

    ResourceManager::SRC imgSrc_;
};

