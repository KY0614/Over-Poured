#include <DxLib.h>
#include "../Utility/AsoUtility.h"
#include "../Manager/SceneManager.h"
#include "Common/Transform.h"
#include "Planet.h"

Planet::Planet(const Stage::NAME& name, const TYPE& type, const Transform& transform)
{

	name_ = name;
	type_ = type;
	transform_ = transform;

	gravityPow_ = 0.0f;
	gravityRadius_ = 0.0f;
	deadLength_ = 0.0f;

}

Planet::~Planet(void)
{
}

void Planet::Init(void)
{
	gravityPow_ = DEFAULT_GRAVITY_POW;
	gravityRadius_ = DEFAULT_GRAVITY_RADIUS;
	deadLength_ = DEFAULT_DEAD_LENGTH;
}

void Planet::Update(void)
{
}

void Planet::Draw(void)
{
    MV1DrawModel(transform_.modelId);

	// èdóÕîÕàÕ
		//DrawSphere3D(
		//	transform_.pos, gravityRadius_, 10, 0xff0000, 0xff0000, false);
		//DrawSphere3D(
		//	transform_.pos, gravityRadius_ + deadLength_, 10, 0xffffff, 0xffffff, false);
}

void Planet::SetPosition(const VECTOR& pos)
{
    transform_.pos = pos;
    transform_.Update();
}

void Planet::SetRotation(const Quaternion& rot)
{
	transform_.quaRot = rot;
	transform_.Update();
}

float Planet::GetGravityPow(void) const
{
	return gravityPow_;
}

void Planet::SetGravityPow(float pow)
{
	gravityPow_ = pow;
}

float Planet::GetGravityRadius(void) const
{
	return gravityRadius_;
}

void Planet::SetGravityRadius(float radius)
{
	gravityRadius_ = radius;
}

const Planet::TYPE& Planet::GetType(void) const
{
	return type_;
}

bool Planet::InRangeGravity(const VECTOR& pos) const
{
	VECTOR diff = VSub(pos, transform_.pos);
	float dis = AsoUtility::SqrMagnitudeF(diff);
	float radius = pow(gravityRadius_, 2);

	if (dis < radius)
	{
		return true;
	}
	else
	{
		return false;
	}
}

bool Planet::InRangeDead(const VECTOR& pos) const
{
	VECTOR diff = VSub(pos, transform_.pos);
	float dis = AsoUtility::SqrMagnitudeF(diff);
	float r = gravityRadius_ + deadLength_;
	float radius = pow(r , 2);

	if (dis < radius)
	{
		return false;
	}
	else
	{
		return true;
	}
}

void Planet::SetDeadLength(float len)
{
	deadLength_ = len;
}

const Stage::NAME& Planet::GetName(void) const
{
	return name_;
}
