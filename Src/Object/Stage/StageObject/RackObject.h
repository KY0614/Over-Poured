#pragma once
#include "../StageObject.h"

class GaugeUI;

class RackObject : public StageObject
{
public:

	static constexpr int RACK_STOCK_MAX = 3;

	RackObject(const std::string objId, const float width,
		const float height, const float depth, Player& player);

	~RackObject(void) = default;

	void PickUp(std::string rackName,std::vector<std::unique_ptr<StageObject>>& object) override;

	void AddStock(int addStockNum) override;

	void Init(VECTOR pos, float rotY = 0.0f) override;

	void Update(void) override;

	void Draw(void) override;

private:

	//ÉQÅ[ÉWÇÃUI
	std::unique_ptr<GaugeUI> gaugeUI_;
};

