#pragma once
#include "../StageObject.h"

class CupLid : public StageObject
{
public:

	CupLid(const std::string objId, const float width,
		const float height, const float depth, Player& player,
		StageObject& object);

	~CupLid(void) = default;

	void Update(void)override;

	StageObject& GetCoffee(void) const { return coffee_; }

private:

	StageObject& coffee_;
};

