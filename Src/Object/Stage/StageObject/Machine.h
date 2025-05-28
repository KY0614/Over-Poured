#pragma once
#include "../Object/Stage/StageObject.h"

class Machine : public StageObject
{
public:

	Machine(const std::string objId, const float width,
		const float height, const float depth,Player& player);
	~Machine(void) = default;

	//必要であれば、UpdateやDrawもオーバーライド
	void Init(void) override;
	void Update(void) override;
	void Draw(void) override;

	void Interact(const std::string& objId, std::vector<std::unique_ptr<StageObject>>& object) override;

	void UpdateInActive(void)override;
	void UpdateActive(void)override;

private:

};

