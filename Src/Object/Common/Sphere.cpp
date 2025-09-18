#include <DxLib.h>
#include "Transform.h"
#include "Sphere.h"

Sphere::Sphere(const Transform& parent) : transformParent_(parent)
{
	radius_ = 0.0f;
	localPos_ = { 0.0f, 0.0f, 0.0f };
}

Sphere::~Sphere(void)
{
}

void Sphere::Draw(void)
{
	//ワールド座標を取得
	VECTOR pos = GetPos();
	//描画
	DrawSphere3D(pos, radius_, DIV_NUM, COLOR, COLOR, false);
}

void Sphere::Draw(int col)
{
	//ワールド座標を取得
	VECTOR pos = GetPos();
	//色を指定して描画
	DrawSphere3D(pos, radius_, DIV_NUM, col, col, false);
}

VECTOR Sphere::GetRotPos(const VECTOR& localPos) const
{
	//相対座標を親の回転に合わせて回転させる
	VECTOR localRotPos = transformParent_.quaRot.PosAxis(localPos);
	//親の位置を足してワールド座標に変換して返す
	return VAdd(transformParent_.pos, localRotPos);
}