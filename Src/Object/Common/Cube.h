#pragma once
#include <DxLib.h>

class Transform;

class Cube
{
public:

	//�f�o�b�O���̊ȈՃJ�v�Z���\���̐F
	static constexpr int COLOR = 0xffffff;

	//�R���X�g���N�^
	//�J�v�Z��������I�u�W�F�N�g���w�肷��
	Cube(const Transform& parent);
	Cube(const Cube& base, const Transform& parent);

	//�f�X�g���N�^
	~Cube(void);

	void MakeCube(VECTOR center, float size, COLOR_U8  col);
	void MakeBox(VECTOR center, float width, float height, float depth, COLOR_U8  col);

	//�`��
	void Draw(void);

private:
	// ���_�z��i�ő�12���̎O�p�` �� 36���_�j
	VERTEX3D Vertex[36];

	//�J�v�Z�����������鑊��
	const Transform& transformParent_;

	//�eTransform����̑��Έʒu(�㑤)
	VECTOR localPosTop_;

	//�eTransform����̑��Έʒu(����)
	VECTOR localPosDown_;

	//���a
	float radius_;

};

