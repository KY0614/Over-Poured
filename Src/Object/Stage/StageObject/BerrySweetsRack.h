#pragma once
#include "../StageObject.h"

class BerrySweetsRack : public StageObject
{
public:

	BerrySweetsRack(const std::string objId, const float width,
		const float height, const float depth, Player& player);

	~BerrySweetsRack(void) = default;

	void PickUp(std::string rackName, std::vector<std::unique_ptr<StageObject>>& object) override;

private:
};

