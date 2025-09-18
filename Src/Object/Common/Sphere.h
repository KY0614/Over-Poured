#pragma once
#include <DxLib.h>

class Transform;

class Sphere
{
public:
	//���̂̐F
	static constexpr int COLOR = 0x000000;

	//���̂̕�����
	static constexpr int DIV_NUM = 5;

	//�R���X�g���N�^
	//���̂�����I�u�W�F�N�g���w�肷��
	Sphere(const Transform& parent);


	//�f�X�g���N�^
	~Sphere(void);

	/// <summary>
	/// �`��
	/// </summary>
	void Draw(void);

	/// <summary>
	/// �w�肵���F�ŋ��̂�`��
	/// </summary>
	/// <param name="col">�w�肷��F</param>
	void Draw(int col);

	/// <summary>
	/// �eTransform����̑��Έʒu���擾
	/// </summary>
	/// <returns>�eTransform����̑��΍��W</returns>
	VECTOR GetLocalPos(void) const { return localPos_; }

	/// <summary>
	/// �eTransform����̑��΍��W���Z�b�g
	/// </summary>
	/// <param name="pos">�ݒ肷�鑊�΍��W</param>
	void SetLocalPos(const VECTOR& pos) { localPos_ = pos; }

	/// <summary>
	/// ���[���h���W���擾
	/// </summary>
	/// <returns>���΍��W�̃��[���h���W</returns>
	VECTOR GetPos(void) const { return GetRotPos(localPos_); }

	/// <summary>
	/// ���΍��W����]�����ă��[���h���W�Ŏ擾����
	/// </summary>
	/// <param name="localPos">���΍��W</param>
	/// <returns>���΍��W�̃��[���h���W</returns>
	VECTOR GetRotPos(const VECTOR& localPos) const;

	/// <summary>
	/// ���̂̔��a���擾
	/// </summary>
	/// <returns>���a�̒l</returns>
	float GetRadius(void) const { return radius_; }

	/// <summary>
	/// ���a��ݒ�
	/// </summary>
	/// <param name="radius">�ݒ肷�锼�a�̒l</param>
	void SetRadius(float radius) { radius_ = radius; }

private:

	//���̂��������鑊��
	const Transform& transformParent_;

	//�eTransform����̑��Έʒu
	VECTOR localPos_;

	//���a
	float radius_;
};

