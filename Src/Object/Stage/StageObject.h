#pragma once
#include <string>
#include "../ActorBase.h"
#include "StageObjectLibrary.h"

class Player;

class StageObject : public ActorBase
{
public:

	StageObject(const std::string objId);

	~StageObject(void);

	void Init(void)override;
	void Update(void)override;
	void Draw(void)override;

	void SetPos(VECTOR pos) { transform_.pos = pos; };
	VECTOR GetPos(void)const { return transform_.pos; };

	bool Interact(Player& player);

private:
	std::string objId_;
	StageObjectLibrary::ObjectParams param_;
	std::pair<std::string, StageObjectLibrary::ObjectParams> object_;


	float fillProgress_ = 0.0f;

	void UpdateDebugImGui(void);
};

