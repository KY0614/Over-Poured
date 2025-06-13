#pragma once
#include "../StageObject.h"

class RackObject : public StageObject
{
public:

	RackObject(const std::string objId, const float width,
		const float height, const float depth, Player& player);

	~RackObject(void) = default;

	void PickUp(std::string rackName,std::vector<std::unique_ptr<StageObject>>& object) override;

private:
};

