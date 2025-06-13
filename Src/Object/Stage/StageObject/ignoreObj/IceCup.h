#pragma once
#include "../StageObject.h"

class IceCup : public StageObject
{
public:

	IceCup(const std::string objId, const float width,
		const float height, const float depth, Player& player);

	~IceCup(void) = default;

	void ItemCarry(void) override;

	void ItemPlaced(VECTOR pos)override;

	void Draw(void)override;

	void PouredIce(void);

	bool IsIce(void)const { return isIced_; }

private:

	bool isIced_;

	void UpdatePlaced(void)override;
	void UpdateHold(void)override;
};

