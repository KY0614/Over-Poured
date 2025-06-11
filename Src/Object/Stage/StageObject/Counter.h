#pragma once
#include "../StageObject.h"

class Counter : public StageObject
{
public:

	Counter(const std::string objId, const float width,
		const float height, const float depth, Player& player,
		std::vector<std::unique_ptr<StageObject>>& objects);

	~Counter(void) = default;

	void Update(void) override;

	void Interact(const std::string& objId) override;

private:
	std::vector<std::unique_ptr<StageObject>>& objects_;
};

