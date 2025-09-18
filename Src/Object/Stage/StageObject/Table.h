#pragma once
#include "../StageObject.h"

class Table : public StageObject
{
public:

	Table(const std::string objId,
		const float height, Player& player,
		std::vector<std::unique_ptr<StageObject>>& objects);

	~Table(void) = default;

	void Update(void)override;

private:

	std::vector<std::unique_ptr<StageObject>>& objects_;

};

