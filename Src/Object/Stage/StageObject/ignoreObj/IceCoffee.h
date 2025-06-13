#pragma once
#include "../StageObject.h"

class IceCoffee : public StageObject
{
public:

	IceCoffee(const std::string objId, const float width,
		const float height, const float depth, Player& player);

	~IceCoffee(void) = default;

	void ItemCarry(void) override;

	void ItemPlaced(VECTOR pos)override;

private:

	void UpdatePlaced(void)override;
	void UpdateHold(void)override;
};

