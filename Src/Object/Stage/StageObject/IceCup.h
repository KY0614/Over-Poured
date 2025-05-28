#pragma once
#include "../Object/Stage/StageObject.h"

class IceCup : public StageObject
{
public:

	IceCup(const std::string objId, const float width,
		const float height, const float depth, Player& player);

	~IceCup(void) = default;

	void ItemCarry(void) override;

	void ItemPlaced(VECTOR pos)override;

private:

	void UpdatePlaced(void)override;
	void UpdateHold(void)override;
};

