#pragma once
#include "StageObject.h"

class CupIce : public StageObject
{
public:

	CupIce(const std::string objId, const float width,
		const float height, const float depth, Player& player);

	~CupIce(void) = default;

	void ItemCarry(void) override;

	void Interact(void) override;

private:

	void UpdatePlaced(void)override;
	void UpdateHold(void)override;
};

