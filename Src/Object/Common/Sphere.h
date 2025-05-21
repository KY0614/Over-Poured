#pragma once
#include <DxLib.h>

class Transform;

class Sphere
{
public:
	//�f�o�b�O���̊ȈՃJ�v�Z���\���̐F
	static constexpr int COLOR = 0xffffff;

	//�R���X�g���N�^
	//���̂�����I�u�W�F�N�g���w�肷��
	Sphere(const Transform& parent);
	Sphere(const Sphere& base, const Transform& parent);

	//�f�X�g���N�^
	~Sphere(void);

	//�`��
	void Draw(void);

	//�eTransform����̑��Έʒu���擾
	VECTOR GetLocalPos(void) const { return localPos_; }

	//�eTransform����̑��Έʒu���Z�b�g
	void SetLocalPos(const VECTOR& pos) { localPos_ = pos; }

	//���[���h���W���擾
	VECTOR GetPos(void) const { return GetRotPos(localPos_); }

	//���΍��W����]�����ă��[���h���W�Ŏ擾����
	VECTOR GetRotPos(const VECTOR& localPos) const;

	//���a
	float GetRadius(void) const { return radius_; }
	void SetRadius(float radius) { radius_ = radius; }

	//����
	float GetHeight(void) const { return localPos_.y; }

	//���̂̒��S���W
	//VECTOR GetCenter(void) const;

private:

	//���̂��������鑊��
	const Transform& transformParent_;

	//�eTransform����̑��Έʒu
	VECTOR localPos_;

	//���a
	float radius_;

};

