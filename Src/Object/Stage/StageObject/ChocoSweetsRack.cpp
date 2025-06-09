#include "ChocoSweetsRack.h"

ChocoSweetsRack::ChocoSweetsRack(const std::string objId, const float width,
	const float height, const float depth, Player& player,
	std::vector<std::unique_ptr<StageObject>>& objects) :
	StageObject(objId, width, height, depth, player)
{
}

void ChocoSweetsRack::ItemCarry(void)
{
}

void ChocoSweetsRack::ItemPlaced(VECTOR pos)
{
}

void ChocoSweetsRack::UpdatePlaced(void)
{
}

void ChocoSweetsRack::UpdateHold(void)
{
}
