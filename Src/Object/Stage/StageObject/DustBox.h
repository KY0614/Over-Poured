#pragma once
#include "../StageObject.h"

class DustBox : public StageObject
{
public:

	DustBox(const std::string objId,Player& player,
		std::vector<std::unique_ptr<StageObject>>& objects);

	~DustBox(void) = default;

	void Interact(const std::string& objId) override;

private:

	std::vector<std::unique_ptr<StageObject>>& objects_;
};

