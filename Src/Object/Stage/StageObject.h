#pragma once
#include <string>
#include <functional>
#include "../ActorBase.h"
#include "StageObjectLibrary.h"

class Cube;
class Player;

class StageObject : public ActorBase
{
public:

	StageObject(const std::string objId,const float width,
		const float height, const float depth);

	~StageObject(void);

	void Init(void)override;
	void Update(void)override;
	void Draw(void)override;

	void SetPos(VECTOR pos);
	VECTOR GetPos(void)const { return transform_.pos; };

	bool Interact(Player& player);

private:
	std::string objId_;
	StageObjectLibrary::ObjectParams param_;
	std::pair<std::string, StageObjectLibrary::ObjectParams> object_;

	using ObjectUpdateTable_t = std::function<void(void)>;
	ObjectUpdateTable_t objectUpdate_;

	//‰¼ƒ‚ƒfƒ‹‚Ì—§•û‘Ì
	std::unique_ptr<Cube> cube_;
	float width_;
	float height_;
	float depth_;

	float fillProgress_ = 0.0f;

	/// <summary>
	/// À•W‚âŠg‘å—¦‚ğ’²®‚·‚é—p‚ÌGUI
	/// </summary>
	/// <param name=""></param>
	void UpdateDebugImGui(void);
};

