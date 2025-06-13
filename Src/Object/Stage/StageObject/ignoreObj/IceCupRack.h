#pragma once
#include "../StageObject.h"

class IceCupRack : public StageObject
{
public:

	IceCupRack(const std::string objId, const float width,
		const float height, const float depth, Player& player);

	~IceCupRack(void) = default;

	void PickUp(std::string rackName,std::vector<std::unique_ptr<StageObject>>& object) override;

private:
};

