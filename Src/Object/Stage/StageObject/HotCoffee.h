#pragma once
#include "../Object/Stage/StageObject.h"
class HotCoffee : public StageObject
{
public:

	HotCoffee(const std::string objId, const float width,
		const float height, const float depth, Player& player);

	~HotCoffee(void) = default;

	void ItemCarry(void) override;

	void ItemPlaced(VECTOR pos)override;

private:

	void UpdatePlaced(void)override;
	void UpdateHold(void)override;
};

