#pragma once
#include "../StageObject.h"

class Sweets : public StageObject
{
public:

	Sweets(const std::string objId, const float width,
		const float height, const float depth, Player& player);

	~Sweets(void) = default;

	void ItemCarry(void) override;

	void ItemPlaced(VECTOR pos)override;

private:

	void UpdatePlaced(void)override;
	void UpdateHold(void)override;
};

