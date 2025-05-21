#include <DxLib.h>
#include "Transform.h"
#include "Sphere.h"

Sphere::Sphere(const Transform& parent) : transformParent_(parent)
{
	radius_ = 0.0f;
	localPos_ = { 0.0f, 0.0f, 0.0f };
}

Sphere::Sphere(const Sphere& base, const Transform& parent) : transformParent_(parent)
{
	radius_ = base.GetRadius();
	localPos_ = base.GetLocalPos();
}

Sphere::~Sphere(void)
{
}

void Sphere::Draw(void)
{
	//��̋���
	VECTOR pos = GetPos();
	DrawSphere3D(pos, radius_, 5, COLOR, COLOR, false);
}

VECTOR Sphere::GetRotPos(const VECTOR& localPos) const
{
	VECTOR localRotPos = transformParent_.quaRot.PosAxis(localPos);
	return VAdd(transformParent_.pos, localRotPos);
}