#pragma once
#include "../StageObject.h"

class ChocoSweetsRack : public StageObject
{
public:

	ChocoSweetsRack(const std::string objId, const float width,
		const float height, const float depth, Player& player,
		std::vector<std::unique_ptr<StageObject>>& objects);

	~ChocoSweetsRack(void) = default;

	void ItemCarry(void) override;

	void ItemPlaced(VECTOR pos)override;

private:

	void UpdatePlaced(void)override;
	void UpdateHold(void)override;
};

