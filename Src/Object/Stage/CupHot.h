#pragma once
#include "StageObject.h"

class CupHot : public StageObject
{
public:

	CupHot(const std::string objId, const float width,
		const float height, const float depth, Player& player);

	~CupHot(void) = default;

	void Interact(void) override;

private:

	void UpdatePlaced(void)override;
	void UpdateHold(void)override;

};

