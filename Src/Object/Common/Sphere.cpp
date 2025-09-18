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
	//���[���h���W���擾
	VECTOR pos = GetPos();
	//�`��
	DrawSphere3D(pos, radius_, DIV_NUM, COLOR, COLOR, false);
}

void Sphere::Draw(int col)
{
	//���[���h���W���擾
	VECTOR pos = GetPos();
	//�F���w�肵�ĕ`��
	DrawSphere3D(pos, radius_, DIV_NUM, col, col, false);
}

VECTOR Sphere::GetRotPos(const VECTOR& localPos) const
{
	//���΍��W��e�̉�]�ɍ��킹�ĉ�]������
	VECTOR localRotPos = transformParent_.quaRot.PosAxis(localPos);
	//�e�̈ʒu�𑫂��ă��[���h���W�ɕϊ����ĕԂ�
	return VAdd(transformParent_.pos, localRotPos);
}