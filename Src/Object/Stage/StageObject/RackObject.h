#pragma once
#include "../StageObject.h"

class GaugeUI;

class RackObject : public StageObject
{
public:

	static constexpr float UI_OFFSET_Y = 30.0f;

	RackObject(const std::string objId, const float width,
		const float height, const float depth, Player& player);

	~RackObject(void) = default;

	void PickUp(std::string rackName,std::vector<std::unique_ptr<StageObject>>& object) override;

	void AddStock(int addStockNum) override;

	void Init(VECTOR pos, float rotY = 0.0f) override;

	void Update(void) override;

	void Draw(void) override;

private:

	//�I�u�W�F�N�g�̎c���
	int sweetsStockCnt_;	//�X�C�[�c�p
	int cupsStockCnt_;		//�J�b�v�p

	float addInterval_;	//�ǉ������Ԋu

	//�Q�[�W��UI
	std::unique_ptr<GaugeUI> gaugeUI_;

	Transform sweetsOfRack_[SWEETS_STOCK_MAX];
	Transform cupesOfRack_[CUP_STOCK_MAX];
};

