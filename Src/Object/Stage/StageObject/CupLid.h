#pragma once
#include "../Object/Stage/StageObject.h"

class CupLid : public StageObject
{
public:

	CupLid(const std::string objId, const float width,
		const float height, const float depth, Player& player);

	~CupLid(void) = default;

	void Update(void)override;

private:
};

