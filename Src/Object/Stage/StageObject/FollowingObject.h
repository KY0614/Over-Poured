#pragma once
#include "../StageObject.h"

class FollowingObject : public StageObject
{
public:
	//�R���X�g���N�^
	FollowingObject(const std::string objId,Player& player,
		StageObject& followObject);
	//�f�X�g���N�^
	~FollowingObject(void) = default;

	/// <summary>
	/// ����������
	/// </summary>
	/// <param name="pos">����������W</param>
	/// <param name="rotY">��]�p�iY���j</param>
	/// <param name="scale">�傫��</param>
	void Init(VECTOR pos, float rotY = 0.0f, VECTOR scale = { 1.0f,1.0f,1.0f })override;

	/// <summary>
	/// �X�V����
	/// </summary>
	void Update(void)override;

	/// <summary>
	/// �Ǐ]���Ă���I�u�W�F�N�g���擾
	/// </summary>
	/// <param name=""></param>
	/// <returns>�Ǐ]���̃I�u�W�F�N�g</returns>
	const StageObject& GetFollowedObj(void) const { return followObj_; }

private:
	//�Ǐ]���̃I�u�W�F�N�g
	StageObject& followObj_;
};

