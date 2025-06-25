#include "FollowingObject.h"

FollowingObject::FollowingObject(const std::string objId,
	const float width, const float height,
	const float depth, Player& player,
	StageObject& followObject) : 
	StageObject(objId, width, height, depth, player), followObj_(followObject)
{
}

void FollowingObject::Init(VECTOR pos)
{
}

void FollowingObject::Update(void)
{
	VECTOR pos = followObj_.GetTransform().pos;
	pos.y += followObj_.GetObjHeight();

	transform_.pos = pos;

	transform_.Update();
}
