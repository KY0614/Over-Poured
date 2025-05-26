#pragma once
#include "StageObject.h"

class Machine : public StageObject
{
public:

	Machine(const std::string objId, const float width,
		const float height, const float depth,Player& player,
		StageObject& cupH,StageObject& cupI);
	~Machine(void) = default;

	//必要であれば、UpdateやDrawもオーバーライド
	void Init(void) override;
	void Update(void) override;
	void Draw(void) override;

	void Carry(void) override;

	void Interact(void) override;

private:

	StageObject& cupH_;
	StageObject& cupI_;

};

