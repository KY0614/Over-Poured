#pragma once
#include "../StageObject.h"

class ChocoSweetsRack : public StageObject
{
public:

	ChocoSweetsRack(const std::string objId, const float width,
		const float height, const float depth, Player& player);

	~ChocoSweetsRack(void) = default;

	void PickUp(std::string rackName, std::vector<std::unique_ptr<StageObject>>& object) override;

private:
};

