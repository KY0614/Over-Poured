#include "../../Common/Sphere.h"
#include "FollowingObject.h"

namespace
{
	//球のローカル座標
	const VECTOR SPHERE_LOCAL_POS = { 0.0f,30.0f,0.0f };
}

FollowingObject::FollowingObject(const std::string objId,Player& player,
	StageObject& followObject) : 
	StageObject(objId, player), followObj_(followObject)
{
}

void FollowingObject::Init(VECTOR pos, float rotY, VECTOR scale)
{
	//初期化
	StageObject::Init(pos, rotY, scale);

	//テーブルとオブジェクトの判定の邪魔にならないように
	//球体の半径は０にする
	param_.collisionRadius_ = 0.0f;
	sphere_->SetLocalPos(SPHERE_LOCAL_POS);
	sphere_->SetRadius(param_.collisionRadius_);
}

void FollowingObject::Update(void)
{
	//追従するオブジェクトの位置に合わせる
	transform_.pos = followObj_.GetTransform().pos;
	//モデル情報の更新
	transform_.Update();
}
