#include "../../Common/Sphere.h"
#include "FollowingObject.h"

namespace
{
	//���̃��[�J�����W
	const VECTOR SPHERE_LOCAL_POS = { 0.0f,30.0f,0.0f };
}

FollowingObject::FollowingObject(const std::string objId,Player& player,
	StageObject& followObject) : 
	StageObject(objId, player), followObj_(followObject)
{
}

void FollowingObject::Init(VECTOR pos, float rotY, VECTOR scale)
{
	//������
	StageObject::Init(pos, rotY, scale);

	//�e�[�u���ƃI�u�W�F�N�g�̔���̎ז��ɂȂ�Ȃ��悤��
	//���̂̔��a�͂O�ɂ���
	param_.collisionRadius_ = 0.0f;
	sphere_->SetLocalPos(SPHERE_LOCAL_POS);
	sphere_->SetRadius(param_.collisionRadius_);
}

void FollowingObject::Update(void)
{
	//�Ǐ]����I�u�W�F�N�g�̈ʒu�ɍ��킹��
	transform_.pos = followObj_.GetTransform().pos;
	//���f�����̍X�V
	transform_.Update();
}
