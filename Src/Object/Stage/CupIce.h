#pragma once
#include "StageObject.h"

class CupIce :
    public StageObject
{
public:

	CupIce(const std::string objId, const float width,
		const float height, const float depth, Player& player);

	~CupIce(void) = default;

	void Carry(void) override;

	void Interact(void) override;

private:
};

