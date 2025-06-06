#include "DustBox.h"

DustBox::DustBox(const std::string objId, const float width,
	const float height, const float depth, Player& player,
	std::vector<std::unique_ptr<StageObject>>& objects) :
	StageObject(objId, width, height, depth, player), objects_(objects)
{
}

void DustBox::Interact(const std::string& objId)
{
}
