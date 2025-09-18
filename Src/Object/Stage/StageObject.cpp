#include "../Libs/ImGui/imgui.h"
#include "../../Common/DebugDrawFormat.h"
#include "../Manager/Generic/ResourceManager.h"
#include "../Common/Sphere.h"
#include "../Common/Cube.h"
#include "../../Utility/AsoUtility.h"
#include "../../Utility/StringUtility.h"
#include "StageObjectLibrary.h"
#include "StageObject.h"

StageObject::StageObject(const std::string objId,
	const float height, Player& player):
	objId_(objId),height_(height),player_(player)
{
	isActioned_ = false;
	itemState_ = ITEM_STATE::NONE;
	machineState_ = MACHINE_STATE::NONE;
	param_ = StageObjectLibrary::ObjectParams();
	isLid_ = false;
	hasStock_ = false;
}

StageObject::~StageObject(void)
{
}

void StageObject::Init(VECTOR pos,float rotY, VECTOR scale)
{
	//�쐬����I�u�W�F�N�g�̃p�����[�^��json�t�@�C������ǂݍ���
	param_ = StageObjectLibrary::LoadData(objId_).second;

	//�������SRC�ɕϊ����ă��f���ݒ�
	ResourceManager::SRC srcType = ResourceManager::GetInstance().StringToSRC(param_.id_);

	//���f���̊�{�ݒ�
	transform_.SetModel(ResourceManager::GetInstance().LoadModelDuplicate(srcType));
	transform_.scl = scale;
	transform_.pos = pos;
	transform_.quaRot = Quaternion();
	transform_.quaRotLocal =
		Quaternion::Euler({ 0.0f, AsoUtility::Deg2RadF(rotY), 0.0f });
	transform_.MakeCollider(Collider::TYPE::STAGE);
	transform_.Update();

	//�����蔻��p�̋����쐬
	sphere_ = std::make_unique<Sphere>(transform_);
	//�I�u�W�F�N�g����̑��΍��W��ݒ�
	sphere_->SetLocalPos(AsoUtility::VECTOR_ZERO);
	//���a��ݒ�
	sphere_->SetRadius(param_.collisionRadius_);

	//�e�[�u���ƃJ�E���^�[�̓����蔻��p�̋��̂̍��������߂ɂ��Ă����i�e�[�u�����甼�����̂��o�邭�炢�j
	if (objId_ == TABLE || objId_ == COUNTER)sphere_->SetLocalPos({ 0.0f, height_, 0.0f });
	//�S�~�������l
	if (objId_ == DUST_BOX)sphere_->SetLocalPos({ 0.0f, height_ + sphere_->GetRadius(), 0.0f });

	//�A�C�e���̏�����Ԃ͐ݒu���
	ChangeItemState(ITEM_STATE::PLACED);

	//�}�V���̏�����Ԃ͔�ғ����
	ChangeMachineState(MACHINE_STATE::INACTIVE);

	//�e�[�u������Ȃ������烂�f����ǂݍ��܂Ȃ�
	if (objId_ != TABLE)return;
	InitTableColliderModel();
}

void StageObject::Update(void)
{
	isActioned_ = false;

	switch (itemState_)
	{
	case StageObject::ITEM_STATE::NONE:
		break;

	case StageObject::ITEM_STATE::PLACED:
		UpdatePlaced();
		break;

	case StageObject::ITEM_STATE::HOLD:
		UpdateHold();
		break;

	default:
		break;
	}

	switch (machineState_)
	{
	case StageObject::MACHINE_STATE::NONE:
		break;

	case StageObject::MACHINE_STATE::INACTIVE:
		UpdateInActive();
		break;

	case StageObject::MACHINE_STATE::ACTIVE:
		UpdateActive();
		break;

	default:
		break;
	}
	tableColliderTran_.Update();
	transform_.Update();
}

void StageObject::Draw(void)
{
	//���f���̕`��
	MV1DrawModel(transform_.modelId);
}

void StageObject::SetPos(VECTOR pos)
{
	transform_.pos = pos;
}

void StageObject::SetScale(VECTOR scale)
{
	transform_.scl = scale;
}

VECTOR StageObject::GetSpherePos(void) const
{
	return sphere_->GetPos();
}

VECTOR StageObject::GetTopCenter(void) const
{
	VECTOR center = transform_.pos;
	center.y = height_;

	return sphere_->GetPos();
	//return center;
}

float StageObject::GetSphereRad(void) const
{
	return sphere_->GetRadius();
}

void StageObject::ItemCarry(void)
{
}

void StageObject::ItemPlaced(VECTOR pos)
{
}

void StageObject::Interact(const std::string& objId)
{
}

void StageObject::PickUp(std::string rackName,std::vector<std::unique_ptr<StageObject>>& object)
{
}

void StageObject::AddStock(void)
{
}

void StageObject::UpdatePlaced(void)
{
}

void StageObject::UpdateHold(void)
{
}

void StageObject::UpdateInActive(void)
{
}

void StageObject::UpdateActive(void)
{
}

void StageObject::InitTableColliderModel(void)
{
	//���f���̊�{�ݒ�i�e�[�u���̃R���C�_�[�p���f��)
	tableColliderTran_.SetModel(ResourceManager::GetInstance().LoadModelDuplicate(
		ResourceManager::SRC::TABLE_COL));
	tableColliderTran_.scl = transform_.scl;
	tableColliderTran_.pos = transform_.pos;
	tableColliderTran_.quaRot = transform_.quaRot;
	tableColliderTran_.quaRotLocal = transform_.quaRotLocal;
	tableColliderTran_.MakeCollider(Collider::TYPE::STAGE);
	tableColliderTran_.Update();
}