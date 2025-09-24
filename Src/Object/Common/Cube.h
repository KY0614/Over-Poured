#pragma once
#include <DxLib.h>

class Transform;

class Cube
{
public:
	//�ő咸�_��
	static constexpr int VERTEX_NUM = 36;	

	//�f�o�b�O���̊ȈՃJ�v�Z���\���̐F
	static constexpr int COLOR = 0xffffff;

	//�R���X�g���N�^
	//�J�v�Z��������I�u�W�F�N�g���w�肷��
	Cube(const Transform& parent);

	//�f�X�g���N�^
	~Cube(void);

	/// <summary>
	/// �����̂��쐬����
	/// </summary>
	/// <param name="center">���S���W</param>
	/// <param name="size">�T�C�Y</param>
	/// <param name="col">�F</param>
	void MakeCube(VECTOR center, float size, COLOR_U8  col);

	/// <summary>
	/// �����̂��쐬����
	/// </summary>
	/// <param name="center">����������W(�����̂̒��S���W�j</param>
	/// <param name="width">��</param>
	/// <param name="height">����</param>
	/// <param name="depth">���s��</param>
	/// <param name="col">�F</param>
	void MakeBox(VECTOR center, float width, float height, float depth, COLOR_U8  col);

private:
	// ���_�z��i�ő�12���̎O�p�` �� 36���_�j
	VERTEX3D Vertex[VERTEX_NUM];

	//�J�v�Z�����������鑊��
	const Transform& transformParent_;

	//�eTransform����̑��Έʒu(�㑤)
	VECTOR localPosTop_;

	//�eTransform����̑��Έʒu(����)
	VECTOR localPosDown_;
};

