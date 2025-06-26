#include "../../../Manager/Generic/ResourceManager.h"
#include "../../../Utility/AsoUtility.h"
#include "../../Common/Sphere.h"
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

	std::string followObj = "";
	if (followObj_.GetParam().id_ == "Hot_Coffee")
	{
		followObj = "Hot_Cup_Lid";
	}
	else if (followObj_.GetParam().id_ == "Ice_Coffee")
	{
		followObj = "Ice_Cup_Lid";
	}
	else if (followObj_.GetParam().id_ == "Cup_With_Ice")
	{
		followObj = "Ice";
	}

	// ï∂éöóÒÇSRCÇ…ïœä∑ÇµÇƒÉÇÉfÉãê›íË
	ResourceManager::SRC srcType = ResourceManager::GetInstance().StringToSRC(followObj);

	//ÉÇÉfÉãÇÃäÓñ{ê›íË
	transform_.SetModel(ResourceManager::GetInstance().LoadModelDuplicate(srcType));
	transform_.scl = AsoUtility::VECTOR_ONE;
	transform_.pos = pos;
	transform_.quaRot = Quaternion();
	transform_.quaRotLocal =
		Quaternion::Euler({ 0.0f, 0.0f, 0.0f });
	transform_.MakeCollider(Collider::TYPE::STAGE);

	rad_ = 0.0f;

	sphere_ = std::make_unique<Sphere>(transform_);
	sphere_->SetLocalPos({ 0.0f, 0.0f, 0.0f });
	sphere_->SetRadius(rad_);

	transform_.Update();
}

void FollowingObject::Update(void)
{
	transform_.pos = followObj_.GetTransform().pos;

	transform_.Update();
}
