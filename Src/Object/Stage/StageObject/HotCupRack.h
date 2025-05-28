#pragma once
#include "../Object/Stage/StageObject.h"

class HotCupRack : public StageObject
{
public:

	HotCupRack(const std::string objId, const float width,
		const float height, const float depth, Player& player);

	~HotCupRack(void) = default;

	void PickUp(std::vector<std::unique_ptr<StageObject>>& object) override;

	void UpdateInActive(void)override;
	void UpdateActive(void)override;

private:

	StageObject* MakeHotCup(void);

};

