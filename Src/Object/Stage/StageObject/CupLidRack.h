#pragma once
#include "../StageObject.h"

class CupLidRack : public StageObject
{
public:

	CupLidRack(const std::string objId, const float width,
		const float height, const float depth, Player& player,
		std::vector<std::unique_ptr<StageObject>>& object);

	~CupLidRack(void) = default;

	void Interact(const std::string& objId) override;

	void Update(void)override;
	void Draw(void)override;

private:

	std::vector<std::unique_ptr<StageObject>>& objects_;
};

