#pragma once
#include "../Customer/CustomerBase.h"

class HotCustomer : public CustomerBase
{
public:
	//�R���X�g���N�^
	HotCustomer(void);

	/// <summary>
	/// �`�揈��
	/// </summary>
	void Draw(void) override;

protected:
	/// <summary>
	/// �p�����[�^�[��ݒ肷��
	/// </summary>
	void SetParam(void) override;

	/// <summary>
	/// �A�j���[�V�����̏���������
	/// </summary>
	void InitAnimation(void) override;
};

