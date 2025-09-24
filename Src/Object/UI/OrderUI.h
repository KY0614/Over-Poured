#pragma once
#include <vector>
#include "UIBase.h"
#include "../Order/Order.h"

class OrderUI :  public UIBase
{
public:
    //�I�[�_�[�̐�
	static constexpr int ORDER_NUM = 2; 

	//�w�iUI�̃T�C�Y
	static constexpr float BACK_IMG_SIZE = 300.0f;  //�w�i�摜�̃T�C�Y
	static constexpr float IMG_SIZE = 100.0f;       //�h�����N�ƃX�C�[�c�̉摜�T�C�Y

    struct OrderUIData
    {
		Order::DRINK drinkType_;        //�h�����N�̎��
		Order::SWEETS sweetsType_;  	//�X�C�[�c�̎��

		int drinkUIImg_;    //�h�����N��UI�摜
		int sweetsUIImg_;   //�X�C�[�c��UI�摜
		int backUIImg_;     //�w�i��UI�摜

		int timerBackImg_;  //�^�C�}�[�̔w�i�摜
		int timerImg_;      //�^�C�}�[�̉摜

		int checkImg_;     //�`�F�b�N�̉摜

		float currentRate_ = 0.0f;	    //���݂̃Q�[�W�̊���
		float displayedRate_ = 0.0f;    //�\������Ă���Q�[�W�̊���
    };

	//�R���X�g���N�^
    OrderUI(Order::DRINK drink,Order::SWEETS sweets,float maxTime);
	//�f�X�g���N�^
    virtual ~OrderUI() = default;

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
    /// �����̎c�莞�Ԃ�ݒ�
    /// </summary>
    /// <param name="progressT">�o�ߎ���</param>
    void SetOrderTimer(const float progressT) { orderTimer_ = progressT; }

	/// <summary>
	/// �`�F�b�N�}�[�N�̏�Ԃ�ݒ�
	/// </summary>
	/// <param name="index">�w�肷��z��</param>
	/// <param name="isCheck">true:�\���@false:��\��</param>
	void SetCheckUI(const int index, const bool isActive) { isOrderCheck_[index] = isActive; }

private:

	//�I�[�_�[��UI�f�[�^
    OrderUIData orderUIData_;

	//�񋟂��ꂽ���i���I�[�_�[�ƈ�v���Ă��邩�ǂ����̃`�F�b�N
    std::vector<bool> isOrderCheck_;

    //�Q�[�W��i�܂���p�̎���
    float gaugeTime_;

	//�Q�[�W�̍ő厞��
    float orderMaxTime_;
	//�Q�[�W�̌��݂̎���
    float orderTimer_;

	//�������e�摜�̃T�C�Y
    float size_;

    /// <summary>
    /// �摜�̓ǂݍ��ݏ���
    /// </summary>
    void LoadImages(void);
};

