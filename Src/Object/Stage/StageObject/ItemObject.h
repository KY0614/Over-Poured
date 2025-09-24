#pragma once
#include "../StageObject.h"

class ItemObject : public StageObject
{
public:
	//�R���X�g���N�^
	ItemObject(const std::string objId, Player& player);
	//�f�X�g���N�^
	~ItemObject(void) = default;

	/// <summary>
	/// �A�C�e���������^�ԏ���
	/// </summary>
	void ItemCarry(void) override;

	/// <summary>
	/// �A�C�e����ݒu���鏈��
	/// </summary>
	/// <param name="pos">�ݒu������W</param>
	void ItemPlaced(VECTOR pos)override;

	/// <summary>
	/// �X�����鏈��
	/// </summary>
	/// <param name=""></param>
	void PouredIce(void);

	/// <summary>
	/// �X�������Ă��邩�ǂ������擾����
	/// </summary>
	/// <param name=""></param>
	/// <returns>�X���肩�ǂ���</returns>
	bool IsIce(void)const { return isIced_; }

	/// <summary>
	/// �`�揈��
	/// </summary>
	void Draw(void) override;

private:

	//�X�������Ă��邩�ǂ���
	bool isIced_;

	/// <summary>
	/// �ݒu��Ԃ̍X�V����
	/// </summary>
	void UpdatePlaced(void)override;

	/// <summary>
	/// ������Ԃ̍X�V����
	/// </summary>
	void UpdateHold(void)override;
};

