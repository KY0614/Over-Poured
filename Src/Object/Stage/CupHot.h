#pragma once
#include "StageObject.h"

class CupHot : public StageObject
{
public:

	CupHot(const std::string objId, const float width, const float height, const float depth);
	~CupHot(void) = default;

	//必要であれば、UpdateやDrawもオーバーライド
	void Init(void) override;
	void Update(void) override;
	void Draw(void) override;

	void Interact(Player& player) override;

private:

};

