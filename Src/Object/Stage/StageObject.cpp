#include "../Manager/Generic/ResourceManager.h"
#include "../Common/Sphere.h"
#include "../../Utility/AsoUtility.h"
#include "../../Utility/StringUtility.h"
#include "StageObjectLibrary.h"
#include "StageObject.h"

StageObject::StageObject(const std::string objId,Player& player):
	objId_(objId),player_(player)
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
	if (objId_ == TABLE || objId_ == COUNTER)sphere_->SetLocalPos({ 0.0f, TABLE_HEIGHT, 0.0f });
	//�S�~�������l
	if (objId_ == DUST_BOX)sphere_->SetLocalPos({ 0.0f, DUST_BOX_HEIGHT, 0.0f });

	//�A�C�e���̏�����Ԃ͐ݒu���
	ChangeItemState(ITEM_STATE::PLACED);

	//�}�V���̏�����Ԃ͔�ғ����
	ChangeMachineState(MACHINE_STATE::INACTIVE);

	//�e�[�u������Ȃ������烂�f����ǂݍ��܂Ȃ�
	if (objId_ != TABLE)return;
	//�e�[�u���̃R���C�_�[�p���f���̏�����
	InitTableColliderModel();
}

void StageObject::Update(void)
{
	//�A�N�V�������ꂽ���ǂ����̃t���O�����Z�b�g
	isActioned_ = false;

	//��Ԃɉ������X�V����
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

	//��Ԃɉ������X�V����
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

	//���f�����̍X�V
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
	//�����蔻��p�̋��̒��S���W��Ԃ�
	return sphere_->GetPos();
}

float StageObject::GetSphereRad(void) const
{	
	//�����蔻��p�̋��̔��a��Ԃ�
	return sphere_->GetRadius();
}

void StageObject::ItemCarry(void)
{	//�A�C�e������Ȃ��I�u�W�F�N�g�͉������Ȃ�
}

void StageObject::ItemPlaced(VECTOR pos)
{	//�A�C�e������Ȃ��I�u�W�F�N�g�͉������Ȃ�
}

void StageObject::Interact(const std::string& objId)
{	//�C���^���N�g�ł��Ȃ��I�u�W�F�N�g�͉������Ȃ�
}

void StageObject::PickUp(std::string rackName,std::vector<std::unique_ptr<StageObject>>& object)
{	//���b�N����Ȃ��I�u�W�F�N�g�͉������Ȃ�
}

void StageObject::AddStock(void)
{	//�C���^���N�g�ł��Ȃ��I�u�W�F�N�g�͉������Ȃ�
}

void StageObject::UpdatePlaced(void)
{	//�A�C�e������Ȃ��I�u�W�F�N�g�͉������Ȃ�
}

void StageObject::UpdateHold(void)
{	//�A�C�e������Ȃ��I�u�W�F�N�g�͉������Ȃ�
}

void StageObject::UpdateInActive(void)
{	//�}�V������Ȃ��I�u�W�F�N�g�͉������Ȃ�
}

void StageObject::UpdateActive(void)
{	//�}�V������Ȃ��I�u�W�F�N�g�͉������Ȃ�
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