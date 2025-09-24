#pragma once
#include "../StageObject.h"

class GaugeUI;
class IconUI;

class RackObject : public StageObject
{
public:
	//UI�̃I�t�Z�b�g�ʒu
	static constexpr float SWEETS_UI_OFFSET_Y = 110.0f;	//�X�C�[�c�p
	static constexpr float CUP_UI_OFFSET_Y = 30.0f;		//�J�b�v�p

	//�R���X�g���N�^
	RackObject(const std::string objId,Player& player);
	//�f�X�g���N�^
	~RackObject(void) = default;

	/// <summary>
	/// ���b�N����I�u�W�F�N�g����鏈��
	/// </summary>
	/// <param name="rackName">�I�u�W�F�N�g��Id</param>
	/// <param name="object">�I�u�W�F�N�g�̃��X�g</param>
	void PickUp(std::string rackName,std::vector<std::unique_ptr<StageObject>>& object) override;

	/// <summary>
	/// /�݌ɂ�ǉ����鏈��
	/// </summary>
	void AddStock(void) override;

	/// <summary>
	/// ����������
	/// </summary>
	/// <param name="pos">����������W</param>
	/// <param name="rotY">��]�p�iY���j</param>
	/// <param name="scale">�傫��</param>
	void Init(VECTOR pos, float rotY = 0.0f, VECTOR scale = { 1.0f,1.0f,1.0f }) override;

	/// <summary>
	/// �X�V����
	/// </summary>
	void Update(void) override;

	/// <summary>
	/// �`�揈��
	/// </summary>
	void Draw(void) override;

private:

	//�I�u�W�F�N�g�̎c���
	int sweetsStockCnt_;	//�X�C�[�c�p
	int cupsStockCnt_;		//�J�b�v�p

	//�ǉ������Ԋu
	float addInterval_;	

	//�Q�[�W��UI
	std::unique_ptr<GaugeUI> gaugeUI_;

	//�A�C�R��UI
	std::unique_ptr<IconUI> iconUI_;
	
	//�݌ɂ��Ȃ��A�C�R��UI
	std::unique_ptr<IconUI> stockIconUI_;

	//���b�N�ɒu�����X�C�[�c�ƃJ�b�v��Transform
	Transform sweetsOfRack_[SWEETS_STOCK_MAX];	//�X�C�[�c�p
	Transform cupesOfRack_[CUP_STOCK_MAX];		//�J�b�v�p
};

