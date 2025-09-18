#include "../../../Manager/Generic/ResourceManager.h"
#include "../../../Utility/AsoUtility.h"
#include "../../Common/Sphere.h"
#include "FollowingObject.h"

FollowingObject::FollowingObject(const std::string objId,
	const float height,Player& player,
	StageObject& followObject) : 
	StageObject(objId, height, player), followObj_(followObject)
{
}

void FollowingObject::Init(VECTOR pos, float rotY, VECTOR scale)
{
	StageObject::Init(pos, rotY, scale);

	//テーブルとオブジェクトの判定の邪魔にならないように
	collisionRad_ = 0.0f;
	sphere_->SetLocalPos({0.0f,30.0f,0.0f});
	sphere_->SetRadius(collisionRad_);
}

void FollowingObject::Update(void)
{
	transform_.pos = followObj_.GetTransform().pos;

	transform_.Update();
}
