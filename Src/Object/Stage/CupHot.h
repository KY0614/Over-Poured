#pragma once
#include "StageObject.h"

class CupHot : public StageObject
{
public:

	CupHot(const std::string objId, const float width,
		const float height, const float depth, Player& player,
		std::vector<std::unique_ptr<StageObject>>& tables);

	~CupHot(void) = default;

	void ItemCarry(void) override;

	void ItemPlaced(void)override;

	void Interact(void) override;

private:

	std::vector<std::unique_ptr<StageObject>>& tables_;

	void UpdatePlaced(void)override;
	void UpdateHold(void)override;

};

