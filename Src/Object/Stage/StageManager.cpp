#include <DxLib.h>
#include "../../Application.h"
#include "../Utility/CommonUtility.h"
#include "../../Manager/GameSystem/SoundManager.h"
#include "../../Manager/Generic/ResourceManager.h"
#include "../../Manager/Generic/InputManager.h"
#include "../Player.h"
#include "../Common/Sphere.h"
#include "../Common/AnimationController.h"
#include "StageObject/Furnitures.h"
#include "StageObject.h"
#include "StageObject/ItemObject.h"
#include "StageObject/RackObject.h"
#include "StageObject/FollowingObject.h"
#include "StageObject/Table.h"
#include "StageObject/Machine.h"
#include "StageObject/CupLidRack.h"
#include "StageObject/DustBox.h"
#include "StageObject/IceDispenser.h"
#include "StageManager.h"

namespace
{
	//�A�j���[�V�����̍Đ����x
	const float ANIM_SPEED = 30.0f; 

	//�e�[�u���֘A
	const int TABLE_COLUMN_NUM = 4;		//�����̃e�[�u���̐��i�P��j
	const int TABLE_ROW_BACK_NUM = 4;	//��O���̃e�[�u���̐��i�P��j
	const int TABLE_ROW_FRONT_NUM = 2;	//�����̃e�[�u���̐��i�h�[�i�c�̃��b�N���ڂ���p�Ȃ̂ŕ\���͂��Ȃ��j
	const int TABLE_CENTER_NUM = 4;		//�����e�[�u���̐�
	const int MAX_TABLE_NUM = 16;		//�e�[�u���̍ő吔
	const float TABLE_WIDTH = 92.5f;	//�e�[�u���̉���

	//���W
	const VECTOR TABLE_POS_BACK = { -150.0f, 0.0f, -270.0f };	//�e�[�u���̍��W(��O���j
	const VECTOR TABLE_POS_FRONT = { -115.0f, 0.0f, 180.0f };	//�e�[�u���̍��W�i�����j
	const VECTOR COLUMN_TABLE_LEFT_POS = { -250.0f, 0.0f, -192.0f };//��i���j�e�[�u���̍��W
	const VECTOR COLUMN_TABLE_RIGHT_POS = { 320.0f, 0.0f, -192.0f };//��(�E�j�e�[�u���̍��W
	const VECTOR CENTER_TABLE_POS = { -20.0f,0.0f,-100.0f };		//��(�E�j�e�[�u���̍��W
	const VECTOR COUNTER_POS = { 225.0f, 0.0f, 190.0f };			//�J�E���^�[�̍��W
	const VECTOR CASE_POS = { -57.0f, 0.0f, 190.0f };				//�V���[�P�[�X�̍��W
	const VECTOR MACHINE_POS = { -128.0f, 76.0f, -175.0f };			//�R�[�q�[�}�V���̍��W
	const VECTOR CUPHOT_POS = { -45.0f, 76.0f, -175.0f };			//�z�b�g�p�J�b�v�̍��W
	const VECTOR CUPICE_POS = { 45.0f, 76.0f, -175.0f };			//�A�C�X�p�J�b�v�̍��W
	const VECTOR ICEDIS_POS = { 133.0f, 76.0f, -175.0f };			//�A�C�X�f�B�X�y���T�[�̍��W
	const VECTOR DUSTBOX_POS = { 240.0f, 0.0f, -270.0f };			//�S�~���̍��W

	const int SOUND_VOLUME = 256; // ���ʉ��̉���(0~256)
}

StageManager::StageManager(Player& player):player_(player)
{
	servedItems_ = {};
	currentOrder_ = {};

	objects_.clear();
	tables_.clear();
	counter_ = nullptr;

	isServed_ = false;
	isServedItems_.clear();

	animationController_ = nullptr;
}

StageManager::~StageManager(void)
{
}

void StageManager::Init(void)
{
	//�T�E���h�̏�����
	InitSound();	

	//3D���f���̏�����
	Init3DModel();
}

void StageManager::Update(void)
{
	//�A�j���[�V�����̍X�V
	animationController_->Update();

	if (animationController_->IsEnd())
	{
		//�A�j���[�V�������I�������}�V�����[�h�ɐ؂�ւ�
		animationController_->Play((int)ANIM_TYPE::IDLE);
	}

	//�X�e�[�W�I�u�W�F�N�g�̍X�V
	for (const auto& obj : objects_)
	{
		obj->Update();
	}
	//�e�[�u���̍X�V
	for (const auto& obj : tables_)
	{
		obj->Update();
	}
	//�J�E���^�[�̃��f�����X�V
	counter_->Update();
	//�Ƌ�̍X�V
	furnitures_->Update();

	//���b�N����J�b�v�����o������
	CupRackInteract();

	//�����^�щ\�ȃI�u�W�F�N�g�̃C���^���N�g����
	CarryableObjInteract();

	//�}�V���Ƃ̃C���^���N�g����
	MachineInteract();

	//�W���b�N�Ƃ̃C���^���N�g����
	LidRackInteract();

	//�S�~���Ƃ̃C���^���N�g����
	DustBoxInteract();

	//3D���f���̍X�V
	registerTran_.Update();
	caseTran_.Update();
}

void StageManager::Draw(void)
{
	//3D���f���̕`��
	//���W�ƃV���[�P�[�X�̃��f��
	MV1DrawModel(registerTran_.modelId);
	MV1DrawModel(caseTran_.modelId);

	//�Ƌ�̕`��
	furnitures_->Draw();

	//�e�[�u���̕`��
	for (const auto& table : tables_)
	{
		table->Draw();
	}

	//�J�E���^�[�̕`��
	counter_->Draw();

	//�X�e�[�W�I�u�W�F�N�g�̕`��
	for (const auto& obj : objects_)
	{
		obj->Draw();
	}
}

void StageManager::SetCurrentOrder(const Order::OrderData& order)
{
	//���������̔z����m�ۂ��A�����������q�̒������e���󂯎��
	size_t size = static_cast<size_t>(order.orderNum_);
	isServedItems_.resize(size);
	for(bool isSuved : isServedItems_)
	{
		isSuved = false; // ������
	}
	currentOrder_ = order;
}

void StageManager::ResetServeData(void)
{
	//�񋟃f�[�^�����Z�b�g
	isServed_ = false;
	servedItems_.drink_ = Order::DRINK::NONE;
	servedItems_.sweets_ = Order::SWEETS::NONE;
	servedItems_.lid_ = false;
	isServedItems_.clear();
	isServedItems_.resize(0);
}

const Transform& StageManager::GetCounterTran(void) const
{
	return counter_->GetTransform(); 
}

const Transform& StageManager::GetTableTran(int index) const
{
	//�͈͊O�̃C���f�b�N�X�͖���
	if (index > static_cast<int>(tables_.size()))return Transform(); 
	return tables_[index]->GetTalbeColTran();
}

const Transform& StageManager::GetShowCase(void) const
{
	return caseTran_;
}

const Transform& StageManager::GetDustBox(void) const
{
	return dustBoxTran_;
}

const Transform& StageManager::GetFloorTran(void) const
{
	return furnitures_->GetFloorTran();
}

void StageManager::Init3DModel(void)
{
	//���f������̊�{���
	//�V���[�P�[�X�̃��f��
	caseTran_.SetModel(
		ResourceManager::GetInstance().LoadModelDuplicate(
			ResourceManager::SRC::SWEETS_CASE));
	caseTran_.scl = CommonUtility::VECTOR_ONE;
	caseTran_.pos = CASE_POS;
	caseTran_.quaRot = Quaternion();
	caseTran_.quaRotLocal = Quaternion();
	caseTran_.MakeCollider(Collider::TYPE::STAGE);
	caseTran_.Update();

	//���W�̃��f��
	registerTran_.SetModel(
		ResourceManager::GetInstance().LoadModelDuplicate(
			ResourceManager::SRC::REGISTER));
	registerTran_.scl = CommonUtility::VECTOR_ONE;
	registerTran_.pos = CommonUtility::VECTOR_ZERO;
	registerTran_.quaRot = Quaternion::Euler(CommonUtility::VECTOR_ZERO);
	registerTran_.quaRotLocal = Quaternion();
	registerTran_.Update();

	//�A�j���[�V�����̏�����
	InitAnimation();

	//�Ƌ�̏�����
	furnitures_ = std::make_unique<Furnitures>();
	furnitures_->Init();

	//�������ł����g��Ȃ��萔
	//�I�u�W�F�N�g�̌���(Y����]�j
	const float rot_R = -90.0f;	//Y��-90�x��]
	const float rot_L = 90.0f;	//Y��90�x��]
	const float rot_B = 180.0f;	//Y��180�x��]

	VECTOR objectPos = {};
	float rotY = 0.0f;

	//���̃e�[�u���Q(��O)
	for (int x = 0; x < TABLE_ROW_BACK_NUM; x++)
	{
		objectPos = TABLE_POS_BACK;
		objectPos.x += (x * TABLE_WIDTH);
		tables_.emplace_back(std::make_unique<Table>(StageObject::TABLE, player_, objects_));
		tables_.back()->Init(objectPos, rot_B);
	}

	//�c�̃e�[�u���Q(�����j
	for (int z = TABLE_ROW_BACK_NUM; z < TABLE_ROW_BACK_NUM + TABLE_COLUMN_NUM; z++)
	{
		objectPos = COLUMN_TABLE_LEFT_POS;
		objectPos.z += ((z - TABLE_ROW_BACK_NUM) * TABLE_WIDTH);
		tables_.emplace_back(std::make_unique<Table>(StageObject::TABLE, player_, objects_));
		tables_.back()->Init(objectPos, rot_R);
	}

	//�c�̃e�[�u���Q(�E���j
	for (int z = TABLE_ROW_BACK_NUM; z < TABLE_ROW_BACK_NUM + TABLE_COLUMN_NUM; z++)
	{
		objectPos = COLUMN_TABLE_RIGHT_POS;
		objectPos.z += ((z - TABLE_ROW_BACK_NUM) * TABLE_WIDTH);
		tables_.emplace_back(std::make_unique<Table>(StageObject::TABLE, player_, objects_));
		tables_.back()->Init(objectPos, rot_L);
	}
	//�e�[�u����X�����̃I�t�Z�b�g�l
	const float tableOffsetX = 90.0f;
	//�^�񒆂̃e�[�u���Q�i�Q�������j
	for (int i = 0; i < TABLE_CENTER_NUM / 2; ++i)
	{
		objectPos = CENTER_TABLE_POS;
		objectPos.x += i * tableOffsetX;
		//�����̃e�[�u���Q��
		tables_.emplace_back(std::make_unique<Table>(StageObject::TABLE, player_, objects_));
		tables_.back()->Init(objectPos);
	}

	//�e�[�u����Z�����̃I�t�Z�b�g�l
	const float tableOffsetZ = 60.0f;
	for (int i = 0; i < TABLE_CENTER_NUM / 2; ++i)
	{
		//�I�u�W�F�N�g�̈ʒu�����������A�I�t�Z�b�g�l�����Z
		objectPos = CENTER_TABLE_POS;
		objectPos.x += i * tableOffsetX;
		objectPos.z += tableOffsetZ;
		//��O���̃e�[�u���Q��
		tables_.emplace_back(std::make_unique<Table>(StageObject::TABLE, player_, objects_));
		tables_.back()->Init(objectPos, rot_B);
	}
	const float caseOffsetX = 20.0f;
	//�P�[�X�̃e�[�u���Q(����)
	for (int x = 0; x < TABLE_ROW_FRONT_NUM; x++)
	{
		objectPos = TABLE_POS_FRONT;
		objectPos.x += (x * (TABLE_WIDTH + caseOffsetX));
		tables_.emplace_back(std::make_unique<Table>(StageObject::TABLE, player_, objects_));
		tables_.back()->Init(objectPos);
	}

	//�J�E���^�[�p�e�[�u��
	counter_ = std::make_unique<Table>(StageObject::COUNTER, player_, objects_);
	counter_->Init(COUNTER_POS);

	VECTOR pos = tables_[TABLE_ROW_BACK_NUM + TABLE_COLUMN_NUM - 1]->GetSpherePos();
	//�z�b�g�p�J�b�v�̃��b�N
	objects_.emplace_back(std::make_unique<RackObject>(StageObject::HOT_CUP_RACK, player_));
	objects_.back()->Init(pos, rot_R);

	//�A�C�X�p�J�b�v�̃��b�N
	pos = tables_[TABLE_ROW_BACK_NUM + TABLE_COLUMN_NUM + TABLE_COLUMN_NUM - 1]->GetSpherePos();
	objects_.emplace_back(std::make_unique<RackObject>(StageObject::ICE_CUP_RACK, player_));
	objects_.back()->Init(pos, rot_L);

	//�`���R�X�C�[�c�p�̃��b�N
	pos = tables_[tables_.size() - 2]->GetSpherePos();
	objects_.emplace_back(std::make_unique<RackObject>(StageObject::CHOCO_SWEETS_RACK, player_));
	objects_.back()->Init(pos);

	//�x���[�X�C�[�c�p�̃��b�N
	pos = tables_.back()->GetSpherePos();
	objects_.emplace_back(std::make_unique<RackObject>(StageObject::BERRY_SWEETS_RACK, player_));
	objects_.back()->Init(pos);

	//�J�b�v�p�̊W
	pos = tables_[2]->GetSpherePos();
	objects_.emplace_back(std::make_unique<CupLidRack>(StageObject::CUP_LID_RACK, player_, objects_));
	objects_.back()->Init(pos);

	//�R�[�q�[�}�V��
	int machineTableIdx = MAX_TABLE_NUM - 2;	//�R�[�q�[�}�V����u���e�[�u���̃C���f�b�N�X
	pos = tables_[machineTableIdx]->GetSpherePos();
	objects_.emplace_back(std::make_unique<Machine>(StageObject::COFFEE_MACHINE, player_, objects_));
	objects_.back()->Init(pos, rot_L);

	//�R�[�q�[�}�V���Q��
	machineTableIdx = MAX_TABLE_NUM - 4;	//�R�[�q�[�}�V����u���e�[�u���̃C���f�b�N�X
	pos = tables_[machineTableIdx]->GetSpherePos();
	objects_.emplace_back(std::make_unique<Machine>(StageObject::COFFEE_MACHINE,player_, objects_));
	objects_.back()->Init(pos, rot_L);

	//�A�C�X�f�B�X�y���T�[
	machineTableIdx = MAX_TABLE_NUM - 1;	//�R�[�q�[�}�V����u���e�[�u���̃C���f�b�N�X
	pos = tables_[machineTableIdx]->GetSpherePos();
	objects_.emplace_back(std::make_unique<IceDispenser>(StageObject::ICE_DISPENSER,player_, objects_));
	objects_.back()->Init(pos, rot_R);

	//�A�C�X�f�B�X�y���T�[�Q��
	machineTableIdx = MAX_TABLE_NUM - 3;	//�R�[�q�[�}�V����u���e�[�u���̃C���f�b�N�X
	pos = tables_[machineTableIdx]->GetSpherePos();
	objects_.emplace_back(std::make_unique<IceDispenser>(StageObject::ICE_DISPENSER,player_, objects_));
	objects_.back()->Init(pos, rot_R);

	//�S�~���̑傫���iy�������������ׂ��j
	const VECTOR dustBoxScale = { 1.0f,0.8f,1.0f };
	//�S�~��
	objects_.emplace_back(std::make_unique<DustBox>(StageObject::DUST_BOX,player_, objects_));
	objects_.back()->Init(DUSTBOX_POS, rot_B, dustBoxScale);
	dustBoxTran_ = objects_.back()->GetTransform(); // �S�~����Transform��ۑ�
}

void StageManager::InitSound(void)
{
	auto& sound = SoundManager::GetInstance();

	//se�ǉ�
	//�I�u�W�F�N�g������SE
	sound.Add(SoundManager::TYPE::SE, SoundManager::SOUND::PICK_UP,
		ResourceManager::GetInstance().Load(ResourceManager::SRC::PICK_UP).handleId_);
	sound.AdjustVolume(SoundManager::SOUND::PICK_UP, SOUND_VOLUME);
	//�I�u�W�F�N�g��u��SE
	sound.Add(SoundManager::TYPE::SE, SoundManager::SOUND::PUT_ON,
		ResourceManager::GetInstance().Load(ResourceManager::SRC::PUT_ON).handleId_);
	sound.AdjustVolume(SoundManager::SOUND::PUT_ON, SOUND_VOLUME);
	//�݌ɂ��[����SE
	sound.Add(SoundManager::TYPE::SE, SoundManager::SOUND::ADD_STOCK,
		ResourceManager::GetInstance().Load(ResourceManager::SRC::ADD_STOCK).handleId_);
	sound.AdjustVolume(SoundManager::SOUND::ADD_STOCK, SOUND_VOLUME);
	//�񋟎���SE
	sound.Add(SoundManager::TYPE::SE, SoundManager::SOUND::PAYING,
		ResourceManager::GetInstance().Load(ResourceManager::SRC::PAYING).handleId_);
	sound.AdjustVolume(SoundManager::SOUND::PAYING, SOUND_VOLUME);
}

void StageManager::InitAnimation(void)
{
	std::string path = Application::PATH_MODEL + "Stage/Animation/";
	animationController_ = std::make_unique<AnimationController>(registerTran_.modelId);
	//�A�j���[�V�����̒ǉ�
	animationController_->Add((int)ANIM_TYPE::CREATE, path + "create_register.mv1", ANIM_SPEED);
	animationController_->Add((int)ANIM_TYPE::PAYING, path + "paying.mv1", ANIM_SPEED);	
	animationController_->Play((int)ANIM_TYPE::IDLE);
}

void StageManager::SurveItem(StageObject& obj)
{
	//obj��ID���Ԃ���OrderData���Z�b�g
	if (obj.GetParam().id_ == StageObject::HOT_COFFEE)
	{
		servedItems_.drink_ = Order::DRINK::HOT;
		servedItems_.lid_ = obj.IsLidOn();
		isServedItems_.front() = true;
	}
	else if (obj.GetParam().id_ == StageObject::ICE_COFFEE)
	{
		servedItems_.drink_ = Order::DRINK::ICE;
		servedItems_.lid_ = obj.IsLidOn();
		isServedItems_.front() = true;
	}

	if (obj.GetParam().id_ == StageObject::CHOCO_SWEETS)
	{
		servedItems_.sweets_ = Order::SWEETS::CHOCO;
		isServedItems_.back() = true;
	}
	else if (obj.GetParam().id_ == StageObject::BERRY_SWEETS)
	{
		servedItems_.sweets_ = Order::SWEETS::STRAWBERRY;
		isServedItems_.back() = true;
	}
	//�񋟌�̓I�u�W�F�N�g���폜
	DeleteSurvedItem(); 

	auto& sound = SoundManager::GetInstance();
	// ������������������
	if (IsOrderCompleted())
	{
		isServed_ = true;
		animationController_->Play((int)ANIM_TYPE::PAYING, false);
		sound.Play(SoundManager::SOUND::PAYING);
	}
}

void StageManager::DeleteSurvedItem(void)
{
	//�J�E���^�[�̋��̂Ɠ������Ă���I�u�W�F�N�g���폜
	for (auto it = objects_.begin(); it != objects_.end(); )
	{
		if (CommonUtility::IsHitSpheres(
			(*it)->GetSpherePos(), (*it)->GetSphereRad(),
			counter_->GetSpherePos(), counter_->GetSphereRad()))
		{
			StageObject* target = it->get(); //������폜����e�I�u�W�F�N�g���L�^
			if (target->IsLidOn())
			{
				//�Ǐ]���Ă���W�����ׂč폜
				for (int i = 0; i < objects_.size(); )
				{
					FollowingObject* follower = dynamic_cast<FollowingObject*>(objects_[i].get());
					if (follower && &(follower->GetFollowedObj()) == target)
					{
						objects_.erase(objects_.begin() + i);
						continue;
					}
					++i;
				}
			}
			it = objects_.erase(it);

		}
		else
		{
			++it;
		}
	}
}

void StageManager::CupRackInteract(void)
{
	auto& pSphere = player_.GetSphere();

	//���b�N����J�b�v�����o������
	for (const auto& obj : objects_)
	{
		//���b�N�ȊO�̃I�u�W�F�N�g�͔��肵�Ȃ�
		if (obj->GetParam().id_ != StageObject::HOT_CUP_RACK &&
			obj->GetParam().id_ != StageObject::ICE_CUP_RACK &&
			obj->GetParam().id_ != StageObject::BERRY_SWEETS_RACK &&
			obj->GetParam().id_ != StageObject::CHOCO_SWEETS_RACK) continue;
		//���b�N�ɍ݌ɂ��Ȃ��Ƃ��̏���
		if (!player_.GetIsHolding() && obj->GetParam().interactable_ &&
			!obj->GetHasStock() &&
			CommonUtility::IsHitSpheres(pSphere.GetPos(), pSphere.GetRadius(),
				obj->GetSpherePos(), obj->GetSphereRad()))
		{
			//�݌ɂ��[���鏈��
			obj->AddStock();
			break;
		}

		//�v���C���[�����������Ă��Ȃ��Ƃ��̏���
		if (!player_.GetIsHolding() && obj->GetParam().interactable_ &&
			CommonUtility::IsHitSpheres(pSphere.GetPos(), pSphere.GetRadius(),
				obj->GetSpherePos(), obj->GetSphereRad()))
		{
			//�݌ɂ�����Ƃ��̓J�b�v�����o������
			obj->PickUp(obj->GetParam().id_, objects_);
			break;
		}
	}

}

void StageManager::CarryableObjInteract(void)
{
	auto& pSphere = player_.GetSphere();
	for (const auto& obj : objects_)
	{
		//�J�E���^�[�ŏ��i��񋟂��鏈��
		if (CommonUtility::IsHitSpheres(obj->GetSpherePos(), obj->GetSphereRad(),
			counter_->GetSpherePos(), counter_->GetSphereRad()
		))
		{
			//�ݒu����Ă���A�C�e����񋟂��鏈��
			if (obj->GetItemState() == StageObject::ITEM_STATE::PLACED)
			{
				SurveItem(*obj);
				break;
			}
		}

		//�v���C���[�����������Ă��Ȃ��Ƃ��̏���
		if (!player_.GetIsHolding() && obj->GetParam().carryable_ &&
			CommonUtility::IsHitSpheres(pSphere.GetPos(), pSphere.GetRadius(),
				obj->GetSpherePos(), obj->GetSphereRad()))
		{
			obj->ItemCarry();
			break;
		}

		//�v���C���[���A�C�e���������Ă���Ƃ��̏���
		if (player_.GetIsHolding())
		{
			//�J�E���^�[�ɐݒu
			if (CommonUtility::IsHitSpheres(obj->GetSpherePos(), obj->GetSphereRad(),
				counter_->GetSpherePos(), counter_->GetSphereRad()
			))
			{
				auto items = counter_->GetParam().acceptedItems_;
				//objId���C���^���N�g�Ώە��ɑ��݂��邩�ǂ���
				bool isAccepted = std::find(items.begin(), items.end(), obj->GetParam().id_) != items.end();
				if (!isAccepted)continue;	//���݂��Ȃ������珈�����Ȃ�
				//�A�C�e����ݒu���鏈��
				obj->ItemPlaced(counter_->GetSpherePos());
			}

			for (const auto& table : tables_)
			{
				//�ݒu�\�ȃe�[�u���̏�ɃA�C�e����ݒu���鏈��
				if (table->GetParam().placeable_ &&
					CommonUtility::IsHitSpheres(pSphere.GetPos(), pSphere.GetRadius(),
						table->GetSpherePos(), table->GetSphereRad()
					))
				{
					//�A�C�e����ݒu���鏈��
					obj->ItemPlaced(table->GetSpherePos());
				}
				//���ɃA�N�V�������s���Ă����烋�[�v�𔲂���
				if (obj->IsActioned())
				{
					break;
				}
			}
		}
		//���ɃA�N�V�������s���Ă����烋�[�v�𔲂���
		if (obj->IsActioned())
		{
			break;
		}
	}
}

void StageManager::MachineInteract(void)
{
	auto& pSphere = player_.GetSphere();

	//�}�V���ƃJ�b�v�̏���
	for (int i = 0; i < static_cast<int>(objects_.size()); ++i)
	{
		//�R�[�q�[�}�V���̔��肾����������
		if (objects_[i]->GetParam().id_ != StageObject::COFFEE_MACHINE)continue;
		//�����Ă���A�C�e�����}�V���ɐݒu���鏈��
		if (player_.GetIsHolding() &&
			CommonUtility::IsHitSpheres(pSphere.GetPos(), pSphere.GetRadius(),
				objects_[i]->GetSpherePos(), objects_[i]->GetSphereRad()))
		{
			objects_[i]->Interact(player_.GetHoldItem());
		}

		//�ݒu���Ĉ�莞�Ԍo������X����J�b�v���o�͂���
		if (objects_[i]->GetParam().interactTime_ <= 0.0f)
		{
			//�R�[�q�[�𐶐����鏈��
			ProduceCoffee(i);
			break;
		}
	}

	//�f�B�X�y���T�[�ƃJ�b�v�̏���
	for (int i = 0; i < static_cast<int>(objects_.size()); ++i)
	{
		//�f�B�X�y���T�[�̔��肾����������
		if (objects_[i]->GetParam().id_ != StageObject::ICE_DISPENSER)continue;
		//�����Ă���A�C�e�����}�V���ɐݒu���鏈��
		if (player_.GetIsHolding() &&
			CommonUtility::IsHitSpheres(pSphere.GetPos(), pSphere.GetRadius(),
				objects_[i]->GetSpherePos(), objects_[i]->GetSphereRad()))
		{
			objects_[i]->Interact(player_.GetHoldItem());
		}

		//�ݒu���Ĉ�莞�Ԍo������X����J�b�v���o�͂���
		if (objects_[i]->GetParam().interactTime_ <= 0.0f)
		{
			//�X����J�b�v�𐶐����鏈��
			DispenseIce2Cup(i);
			break;
		}
	}
}

void StageManager::LidRackInteract(void)
{
	auto& pSphere = player_.GetSphere();
	//�R�[�q�[�ƊW�̃��b�N�Ƃ̏���
	for (const auto& obj : objects_)
	{
		if (obj->GetParam().id_ != StageObject::CUP_LID_RACK)continue;

		//�����Ă���R�[�q�[�ɊW�����鏈��
		//�v���C���[�����������Ă����ԂŊW�̃��b�N�ɋ߂Â����珈������
		if (player_.GetIsHolding() &&
			CommonUtility::IsHitSpheres(pSphere.GetPos(), pSphere.GetRadius(),
				obj->GetSpherePos(), obj->GetSphereRad()))
		{
			//�W�̃��b�N�̃C���^���N�g����
			obj->Interact(player_.GetHoldItem());
		}
		else
		{
			//����O�̏ꍇ�͏����l�ɖ߂�
			obj->IsNotActioned();
			obj->SetInteractTime(StageObject::LID_RACK_INTERACT_TIME);
		}

		//�C���^���N�g�������Ĉ�莞�Ԍo������W������
		if (obj->GetParam().interactTime_ <= 0.0f)
		{
			//�W�����鏈��
			LidFollowCup();
			break;
		}
	}
}

void StageManager::ProduceCoffee(int index)
{
	//�R�[�q�[�}�V���ȊO�̃I�u�W�F�N�g�͔��肵�Ȃ�
	if (objects_[index]->GetParam().id_ != StageObject::COFFEE_MACHINE) return;
	const auto& machine = objects_[index];
	for (int i = 0; i < static_cast<int>(objects_.size()); ++i)
	{
		//�z�b�g�p�J�b�v�ȊO�̃I�u�W�F�N�g�͔��肵�Ȃ�
		if (objects_[i]->GetParam().id_ != StageObject::HOT_CUP &&
			objects_[i]->GetParam().id_ != StageObject::CUP_WITH_ICE) continue;

		//�}�V���̔��肾����������
		if (machine->GetParam().id_ != StageObject::COFFEE_MACHINE)continue;

		//�}�V���̋��̂Ɛݒu����Ă���J�b�v������������
		if (objects_[i]->GetItemState() == StageObject::ITEM_STATE::PLACED &&
			CommonUtility::IsHitSpheres(machine->GetPos(), machine->GetSphereRad(),
				objects_[i]->GetSpherePos(), objects_[i]->GetSphereRad()))
		{
			if (objects_[i]->GetParam().id_ == StageObject::HOT_CUP)
			{
				MakeCoffee(i, *machine, StageObject::HOT_COFFEE);
				return; // �z�b�g�R�[�q�[��������珈�����I��
			}
			else {
				MakeCoffee(i, *machine, StageObject::ICE_COFFEE);
				return; // �A�C�X�R�[�q�[��������珈�����I��
			}
		}

	}
}

void StageManager::MakeCoffee(int index, StageObject& obj, std::string objName)
{
	//�A�C�X�R�[�q�[����鏈��
	if (objName == StageObject::ICE_COFFEE)
	{
		//�X����J�b�v�̏ꍇ�͕X���폜
		ItemObject* cupWithIce = dynamic_cast<ItemObject*>(objects_[index].get());
		if (cupWithIce->IsIce())
		{
			//�W�̃C���f�b�N�X��T��
			for (int i = 0; i < objects_.size(); ++i)
			{
				//dynamic_cast��FollowingObject�^�ɕϊ����A�e�Q�Ƃ��r
				//�W���폜����
				FollowingObject* follower = dynamic_cast<FollowingObject*>(objects_[i].get());
				if (follower && &(follower->GetFollowedObj()) == objects_[index].get())
				{
					objects_.erase(objects_.begin() + i);
					continue; // erase�����̂�i�͐i�߂����̃��[�v��
				}
			}
		}
	}

	//�ݒu����Ă���J�b�v���R�[�q�[�ɏ㏑������
	objects_[index] = std::make_unique<ItemObject>(objName,player_);
	//�}�V���̏�ɏ��悤�ɃJ�b�v��z�u����
	VECTOR cupPos = obj.GetTransform().pos;
	cupPos = VAdd(cupPos,
		{ 0.0f,StageObject::MACHINE_OFSET_Y ,StageObject::MACHINE_OFSET_Z });

	//�}�V���̉�]�ɍ��킹�ăJ�b�v�̈ʒu�𒲐�
	VECTOR rotPos = CommonUtility::RotXZPos(obj.GetTransform().pos, cupPos,
		Quaternion::ToEuler(obj.GetTransform().quaRotLocal).y);
	//�J�b�v���W��ݒ肵�ď�����
	objects_[index]->Init(rotPos);
	//�ݒu��ԂɕύX
	objects_[index]->ChangeItemState(StageObject::ITEM_STATE::PLACED);
}

void StageManager::DispenseIce2Cup(int index)
{
	//�f�B�X�y���T�[�ȊO�̃I�u�W�F�N�g�͔��肵�Ȃ�
	if (objects_[index]->GetParam().id_ != StageObject::ICE_DISPENSER) return;
	const auto& dispenser = objects_[index];
	for (size_t i = 0; i < objects_.size(); ++i)
	{
		//�f�B�X�y���T�[�̔��肾����������
		if (objects_[index]->GetParam().id_ != StageObject::ICE_DISPENSER)continue;

		//�}�V���̋��̂Ɛݒu����Ă���J�b�v������������
		if (objects_[i]->GetItemState() == StageObject::ITEM_STATE::PLACED &&
			CommonUtility::IsHitSpheres(
				dispenser->GetPos(), dispenser->GetSphereRad(),
				objects_[i]->GetSpherePos(), objects_[i]->GetSphereRad()))
		{
			//�ݒu����Ă���J�b�v�ɕX������
			if (auto iceCup = dynamic_cast<ItemObject*>(objects_[i].get()))
			{
				iceCup->PouredIce();

				//�X�𐶐����Ǐ]������
				objects_.emplace_back(std::make_unique<FollowingObject>("Ice",player_, *objects_[i]));
				objects_.back()->Init(CommonUtility::VECTOR_ZERO);
				objects_.back()->Update();
				break;
			}
		}
	}
}

void StageManager::LidFollowCup(void)
{
	for (size_t i = 0; i < objects_.size(); ++i)
	{
		//�R�[�q�[�ȊO�̃I�u�W�F�N�g�͔��肵�Ȃ�
		if (objects_[i]->GetParam().id_ != StageObject::HOT_COFFEE &&
			objects_[i]->GetParam().id_ != StageObject::ICE_COFFEE) continue;

		//���ɊW������Ă���J�b�v�͔��肵�Ȃ�
		if (objects_[i]->IsLidOn())continue;

		for (const auto& lid : objects_)
		{
			//�W�̔��肾����������
			if (lid->GetParam().id_ != StageObject::CUP_LID_RACK)continue;

			//�����Ă���R�[�q�[�ɊW������
			if (objects_[i]->GetItemState()== StageObject::ITEM_STATE::HOLD)
			{
				//�R�[�q�[�I�u�W�F�N�g�ɊW������
				objects_[i]->PutOnTheLid();
				std::string lidType = "";
				if(objects_[i]->GetParam().id_ == StageObject::HOT_COFFEE)lidType = StageObject::HOTCUP_LID;
				else if (objects_[i]->GetParam().id_ == StageObject::ICE_COFFEE)lidType = StageObject::ICECUP_LID;
				//�W�𐶐����Ǐ]������
				objects_.emplace_back(std::make_unique<FollowingObject>(lidType, player_, *objects_[i]));
				objects_.back()->Init(CommonUtility::VECTOR_ZERO);
				objects_.back()->Update();
				break;
			}
		}
	}
}

void StageManager::DustBoxInteract(void)
{
	auto& ins = InputManager::GetInstance();
	auto& pSphere = player_.GetSphere();

	// �S�~���̏���
	for (const auto& obj : objects_)
	{
		if (obj->GetParam().id_ != StageObject::DUST_BOX) continue;

		// �v���C���[�������Ă���A�C�e�����S�~���ɋ߂Â��Ă���ꍇ
		if (player_.GetIsHolding() &&
			CommonUtility::IsHitSpheres(pSphere.GetPos(), pSphere.GetRadius(),
				obj->GetSpherePos(), obj->GetSphereRad()))
		{
			if(ins.IsInputTriggered("Interact"))
			{
				//�S�~���ɃA�C�e�����̂Ă鏈��
				obj->Interact(player_.GetHoldItem());
			}
			break;
		}
	}
}

bool StageManager::IsOrderCompleted(void)
{
	//�S�Ă̒������񋟂��ꂽ���m�F
	for(bool isSurved : isServedItems_)
	{
		if (!isSurved)
		{
			return false; // 1�ł����񋟂̃A�C�e���������false
			break;
		}
	}
	return true;
}