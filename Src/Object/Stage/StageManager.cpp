#include <DxLib.h>
#include "../../Application.h"
#include "../Utility/AsoUtility.h"
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
	isServedItems_.clear();
}

void StageManager::Init(void)
{
	auto& sound = SoundManager::GetInstance();

	//se�ǉ�
	//�A�C�e�������o������SE
	sound.Add(SoundManager::TYPE::SE, SoundManager::SOUND::PICK_UP,
		ResourceManager::GetInstance().Load(ResourceManager::SRC::PICK_UP).handleId_);
	sound.AdjustVolume(SoundManager::SOUND::PICK_UP, 256);	
	//�A�C�e����u������SE
	sound.Add(SoundManager::TYPE::SE, SoundManager::SOUND::PUT_ON,
		ResourceManager::GetInstance().Load(ResourceManager::SRC::PUT_ON).handleId_);
	sound.AdjustVolume(SoundManager::SOUND::PUT_ON, 256);
	//�X�g�b�N�ǉ���SE
	sound.Add(SoundManager::TYPE::SE, SoundManager::SOUND::ADD_STOCK,
		ResourceManager::GetInstance().Load(ResourceManager::SRC::ADD_STOCK).handleId_);
	sound.AdjustVolume(SoundManager::SOUND::ADD_STOCK, 256);
	//�񋟒ǉ���SE
	sound.Add(SoundManager::TYPE::SE, SoundManager::SOUND::PAYING,
		ResourceManager::GetInstance().Load(ResourceManager::SRC::PAYING).handleId_);
	sound.AdjustVolume(SoundManager::SOUND::ADD_STOCK, 256 / 2);

	//3D���f���̏�����
	Init3DModel();
}

void StageManager::Update(void)
{
	animationController_->Update();

	if (animationController_->IsEnd())
	{
		//�A�j���[�V�������I�������}�V�����[�h�ɐ؂�ւ�
		animationController_->Play((int)ANIM_TYPE::IDLE);
	}

	auto& pSphere = player_.GetSphere();

	for (const auto& obj : objects_)
	{
		obj->Update();
	}

	for (const auto& obj : tables_)
	{
		obj->Update();
	}

	counter_->Update();
	furnitures_->Update();

	//���b�N����J�b�v�����o������
	for (const auto& obj : objects_)
	{
		//���b�N�ɍ݌ɂ��Ȃ��Ƃ��̏���
		if (!player_.GetIsHolding() && obj->GetParam().interactable_ &&
			!obj->GetHasStock() &&
			AsoUtility::IsHitSpheres(pSphere.GetPos(), pSphere.GetRadius(),
				obj->GetSpherePos(), obj->GetSphereRad()))
		{
			if (obj->GetParam().id_ == StageObject::HOT_CUP_RACK ||
				obj->GetParam().id_ == StageObject::ICE_CUP_RACK)
			{
				obj->AddStock();
			}
			else if (obj->GetParam().id_ == StageObject::BERRY_SWEETS_RACK ||
				obj->GetParam().id_ == StageObject::CHOCO_SWEETS_RACK)
			{
				obj->AddStock();
			}
			break;
		}

		//�v���C���[�����������Ă��Ȃ��Ƃ��̏���
		if (!player_.GetIsHolding() && obj->GetParam().interactable_ &&
			AsoUtility::IsHitSpheres(pSphere.GetPos(), pSphere.GetRadius(),
				obj->GetSpherePos(), obj->GetSphereRad()))
		{
			obj->PickUp(obj->GetParam().id_, objects_);
			break;
		}
	}

	//�����^�щ\�ȃI�u�W�F�N�g�̃C���^���N�g����
	CarryableObjInteract();

	//�}�V���Ƃ̃C���^���N�g����
	MachineInteract();

	//�W���b�N�Ƃ̃C���^���N�g����
	LidRackInteract();

	DustBoxInteract();

	transform_.Update();
	caseTran_.Update();
}

void StageManager::Draw(void)
{
	MV1DrawModel(transform_.modelId);
	MV1DrawModel(caseTran_.modelId);

	furnitures_->Draw();

	for (const auto& table : tables_)
	{
		table->Draw();
	}

	counter_->Draw();

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

Transform StageManager::GetCounterTran(void) const
{
	return counter_->GetTransform(); 
}

Transform StageManager::GetTableTran(int index) const
{
	if (index > tables_.size())return Transform(); // �͈͊O�̃C���f�b�N�X�͖���
	return tables_[index]->GetTalbeColTran();
}

Transform StageManager::GetShowCase(void) const
{
	return caseTran_;
}

Transform StageManager::GetDustBox(void) const
{
	return dustBoxTran_;
}

Transform StageManager::GetFloorTran(void) const
{
	return furnitures_->GetFloorTran();
}

void StageManager::Init3DModel(void)
{
	//���f������̊�{���
	caseTran_.SetModel(
		ResourceManager::GetInstance().LoadModelDuplicate(
			ResourceManager::SRC::SWEETS_CASE));
	caseTran_.scl = AsoUtility::VECTOR_ONE;
	caseTran_.pos = CASE_POS;
	caseTran_.quaRot = Quaternion::Euler(
		0.0f, 0.0f, 0.0f);

	caseTran_.quaRotLocal = Quaternion();
	caseTran_.MakeCollider(Collider::TYPE::STAGE);
	caseTran_.Update();

	//���f������̊�{���
	transform_.SetModel(
		ResourceManager::GetInstance().LoadModelDuplicate(
			ResourceManager::SRC::REGISTER));
	transform_.scl = AsoUtility::VECTOR_ONE;
	transform_.pos = AsoUtility::VECTOR_ZERO;
	transform_.quaRot = Quaternion::Euler(AsoUtility::VECTOR_ZERO);
	transform_.quaRotLocal = Quaternion();
	transform_.MakeCollider(Collider::TYPE::STAGE);
	transform_.Update();

	InitAnimation(); // �A�j���[�V�����̏�����

	//�Ƌ�̏�����
	furnitures_ = std::make_unique<Furnitures>();
	furnitures_->Init();

	VECTOR firstPos = {};
	//���̃e�[�u���Q(��O)
	for (int x = 0; x < TABLE_ROW_BACK_NUM; x++)
	{
		firstPos = TABLE_POS_BACK;
		firstPos.x += (x * TABLE_WIDTH);
		tables_.emplace_back(std::make_unique<Table>(StageObject::TABLE, 76.0f, player_, objects_));
		tables_.back()->Init(firstPos, 180.0f);
	}

	//�c�̃e�[�u���Q(�����j
	for (int z = TABLE_ROW_BACK_NUM; z < TABLE_ROW_BACK_NUM + TABLE_COLUMN_NUM; z++)
	{
		firstPos = COLUMN_TABLE_LEFT_POS;
		firstPos.z += ((z - TABLE_ROW_BACK_NUM) * TABLE_WIDTH);
		tables_.emplace_back(std::make_unique<Table>(StageObject::TABLE, 76.0f, player_, objects_));
		tables_.back()->Init(firstPos, -90.0f);
	}

	//�c�̃e�[�u���Q(�E���j
	for (int z = TABLE_ROW_BACK_NUM; z < TABLE_ROW_BACK_NUM + TABLE_COLUMN_NUM; z++)
	{
		firstPos = COLUMN_TABLE_RIGHT_POS;
		firstPos.z += ((z - TABLE_ROW_BACK_NUM) * TABLE_WIDTH);
		tables_.emplace_back(std::make_unique<Table>(StageObject::TABLE, 76.0f, player_, objects_));
		tables_.back()->Init(firstPos, 90.0f);
	}

	//�^�񒆂̃e�[�u���Q
	for (int i = 0; i < TABLE_CENTER_NUM / 2; ++i)
	{
		firstPos = CENTER_TABLE_POS;
		firstPos.x += i * 90.0f;
		//�����̃e�[�u���Q��
		tables_.emplace_back(std::make_unique<Table>(StageObject::TABLE, 76.0f, player_, objects_));
		tables_.back()->Init(firstPos, 0.0f);
	}
	for (int i = 0; i < TABLE_CENTER_NUM / 2; ++i)
	{
		firstPos = CENTER_TABLE_POS;
		firstPos.x += i * 90.0f;
		firstPos.z += 60.0f;
		//��O���̃e�[�u���Q��
		tables_.emplace_back(std::make_unique<Table>(StageObject::TABLE, 76.0f, player_, objects_));
		tables_.back()->Init(firstPos, 180.0f);
	}

	//�P�[�X�̃e�[�u���Q(����)
	for (int x = 0; x < TABLE_ROW_FRONT_NUM; x++)
	{
		firstPos = TABLE_POS_FRONT;
		firstPos.x += (x * (TABLE_WIDTH + 20.0f));
		tables_.emplace_back(std::make_unique<Table>(StageObject::TABLE, 76.0f, player_, objects_));
		tables_.back()->Init(firstPos);
	}

	//�J�E���^�[�p�e�[�u��
	counter_ = std::make_unique<Table>(StageObject::COUNTER, 76.0f, player_, objects_);
	counter_->Init(COUNTER_POS);

	VECTOR pos = tables_[TABLE_ROW_BACK_NUM + TABLE_COLUMN_NUM - 1]->GetTopCenter();
	//�z�b�g�p�J�b�v�̃��b�N
	objects_.emplace_back(std::make_unique<RackObject>(StageObject::HOT_CUP_RACK, 20.0f, player_));
	objects_.back()->Init(pos, -90.0f);

	//�A�C�X�p�J�b�v�̃��b�N
	pos = tables_[TABLE_ROW_BACK_NUM + TABLE_COLUMN_NUM + TABLE_COLUMN_NUM - 1]->GetTopCenter();
	objects_.emplace_back(std::make_unique<RackObject>(StageObject::ICE_CUP_RACK, 20.0f, player_));
	objects_.back()->Init(pos, 90.0f);

	//�`���R�X�C�[�c�p�̃��b�N
	pos = tables_[tables_.size() - 2]->GetTopCenter();
	objects_.emplace_back(std::make_unique<RackObject>(StageObject::CHOCO_SWEETS_RACK, 20.0f, player_));
	objects_.back()->Init(pos);

	//�x���[�X�C�[�c�p�̃��b�N
	pos = tables_.back()->GetTopCenter();
	objects_.emplace_back(std::make_unique<RackObject>(StageObject::BERRY_SWEETS_RACK, 20.0f, player_));
	objects_.back()->Init(pos);

	//�J�b�v�p�̊W
	pos = tables_[2]->GetTopCenter();
	objects_.emplace_back(std::make_unique<CupLidRack>(StageObject::CUP_LID_RACK, 20.0f, player_, objects_));
	objects_.back()->Init(pos);

	//�R�[�q�[�}�V��
	pos = tables_[MAX_TABLE_NUM - 2]->GetTopCenter();
	objects_.emplace_back(std::make_unique<Machine>(StageObject::COFFEE_MACHINE, 60.0f, player_, objects_));
	objects_.back()->Init(pos, 90.0f);
	//�R�[�q�[�}�V���Q��
	pos = tables_[MAX_TABLE_NUM - 4]->GetTopCenter();
	objects_.emplace_back(std::make_unique<Machine>(StageObject::COFFEE_MACHINE, 60.0f,player_, objects_));
	objects_.back()->Init(pos, 90.0f);

	//�A�C�X�f�B�X�y���T�[
	pos = tables_[MAX_TABLE_NUM - 1]->GetTopCenter();
	objects_.emplace_back(std::make_unique<IceDispenser>(StageObject::ICE_DISPENSER, 75.0f,
		player_, objects_));
	objects_.back()->Init(pos, -90.0f);
	//�A�C�X�f�B�X�y���T�[�Q��
	pos = tables_[MAX_TABLE_NUM - 3]->GetTopCenter();
	objects_.emplace_back(std::make_unique<IceDispenser>(StageObject::ICE_DISPENSER, 75.0f,
		player_, objects_));
	objects_.back()->Init(pos, -90.0f);

	//�S�~��
	objects_.emplace_back(std::make_unique<DustBox>(StageObject::DUST_BOX, 75.0f,
		player_, objects_));
	objects_.back()->Init(DUSTBOX_POS, -180.0f, { 1.0f,0.8f,1.0f });
	dustBoxTran_ = objects_.back()->GetTransform(); // �S�~����Transform��ۑ�

}

void StageManager::InitAnimation(void)
{
	std::string path = Application::PATH_MODEL + "Stage/";
	animationController_ = std::make_unique<AnimationController>(transform_.modelId);

	animationController_->Add((int)ANIM_TYPE::CREATE, path + "create_register.mv1", 30.0f);

	animationController_->Add((int)ANIM_TYPE::PAYING, path + "paying.mv1", 30.0f);
	animationController_->Play((int)ANIM_TYPE::IDLE);
}

void StageManager::SurveItem(StageObject& obj)
{
	// ���i����OrderData�ɕϊ�
	Order::OrderData data;
	// ��: obj��ID���Ԃ���OrderData���Z�b�g
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
		if (AsoUtility::IsHitSpheres(
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

void StageManager::CarryableObjInteract(void)
{
	auto& pSphere = player_.GetSphere();
	for (const auto& obj : objects_)
	{
		//�J�E���^�[�ŏ��i��񋟂��鏈��
		if (AsoUtility::IsHitSpheres(obj->GetSpherePos(), obj->GetSphereRad(),
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
			AsoUtility::IsHitSpheres(pSphere.GetPos(), pSphere.GetRadius(),
				obj->GetSpherePos(), obj->GetSphereRad()))
		{
			obj->ItemCarry();
			break;
		}

		//�v���C���[���A�C�e���������Ă���Ƃ��̏���
		if (player_.GetIsHolding())
		{
			//�J�E���^�[�ɐݒu
			if (AsoUtility::IsHitSpheres(obj->GetSpherePos(), obj->GetSphereRad(),
				counter_->GetSpherePos(), counter_->GetSphereRad()
			))
			{
				auto items = counter_->GetParam().acceptedItems_;
				//objId���C���^���N�g�Ώە��ɑ��݂��邩�ǂ���
				bool isAccepted = std::find(items.begin(), items.end(), obj->GetParam().id_) != items.end();
				if (!isAccepted)continue;	//���݂��Ȃ������珈�����Ȃ�

				obj->ItemPlaced(counter_->GetTopCenter());
			}

			for (const auto& table : tables_)
			{
				//�ݒu�\�ȃe�[�u���̏�ɃA�C�e����ݒu���鏈��
				if (table->GetParam().placeable_ &&
					AsoUtility::IsHitSpheres(pSphere.GetPos(), pSphere.GetRadius(),
						table->GetSpherePos(), table->GetSphereRad()
					))
				{
					obj->ItemPlaced(table->GetTopCenter());
				}

				if (obj->IsActioned())
				{
					break;
				}
			}
		}
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
	for (size_t i = 0; i < objects_.size(); ++i)
	{
		//�R�[�q�[�}�V���̔��肾����������
		if (objects_[i]->GetParam().id_ != StageObject::COFFEE_MACHINE)continue;
		//�����Ă���A�C�e�����}�V���ɐݒu���鏈��
		if (player_.GetIsHolding() &&
			AsoUtility::IsHitSpheres(pSphere.GetPos(), pSphere.GetRadius(),
				objects_[i]->GetSpherePos(), objects_[i]->GetSphereRad()))
		{
			objects_[i]->Interact(player_.GetHoldItem());
		}

		//�ݒu���Ĉ�莞�Ԍo������X����J�b�v���o�͂���
		if (objects_[i]->GetParam().interactTime_ <= 0.0f)
		{
			ProduceCoffee(static_cast<int>(i));
			break;
		}
	}

	//�f�B�X�y���T�[�ƃJ�b�v�̏���
	for (size_t i = 0; i < objects_.size(); ++i)
	{
		//�f�B�X�y���T�[�̔��肾����������
		if (objects_[i]->GetParam().id_ != StageObject::ICE_DISPENSER)continue;
		//�����Ă���A�C�e�����}�V���ɐݒu���鏈��
		if (player_.GetIsHolding() &&
			AsoUtility::IsHitSpheres(pSphere.GetPos(), pSphere.GetRadius(),
				objects_[i]->GetSpherePos(), objects_[i]->GetSphereRad()))
		{
			objects_[i]->Interact(player_.GetHoldItem());
		}

		//�ݒu���Ĉ�莞�Ԍo������X����J�b�v���o�͂���
		if (objects_[i]->GetParam().interactTime_ <= 0.0f)
		{
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
			AsoUtility::IsHitSpheres(pSphere.GetPos(), pSphere.GetRadius(),
				obj->GetSpherePos(), obj->GetSphereRad()))
		{
			//�W�̃��b�N�̃C���^���N�g����
			obj->Interact(player_.GetHoldItem());
		}
		else
		{
			//����O�̏ꍇ�͏����l�ɖ߂�
			obj->IsNotActioned();	//
			obj->SetInteractTime(StageObject::LID_RACK_INTERACT_TIME);
		}

		//�C���^���N�g�������Ĉ�莞�Ԍo������W������
		if (obj->GetParam().interactTime_ <= 0.0f)
		{
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
	for (size_t i = 0; i < objects_.size(); ++i)
	{
		//�z�b�g�p�J�b�v�ȊO�̃I�u�W�F�N�g�͔��肵�Ȃ�
		if (objects_[i]->GetParam().id_ != StageObject::HOT_CUP &&
			objects_[i]->GetParam().id_ != StageObject::CUP_WITH_ICE) continue;

		//�}�V���̔��肾����������
		if (machine->GetParam().id_ != StageObject::COFFEE_MACHINE)continue;

		//�}�V���̋��̂Ɛݒu����Ă���J�b�v������������
		if (objects_[i]->GetItemState() == StageObject::ITEM_STATE::PLACED &&
			AsoUtility::IsHitSpheres(machine->GetPos(), machine->GetSphereRad(),
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
	//�A�C�X�R�[�q�[�𐶐�����ꍇ�͕X���ɍ폜���Ă���
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
	objects_[index] = std::make_unique<ItemObject>(objName, 30.0f,player_);
	//�}�V���̏�ɏ��悤�ɃJ�b�v��z�u����
	VECTOR cupPos = obj.GetTransform().pos;
	cupPos = VAdd(cupPos,
		{ 0.0f,StageObject::MACHINE_OFSET_Y ,StageObject::MACHINE_OFSET_Z });

	//�}�V���̉�]�ɍ��킹�ăJ�b�v�̈ʒu�𒲐�
	VECTOR rotPos = AsoUtility::RotXZPos(obj.GetTransform().pos, cupPos,
		Quaternion::ToEuler(obj.GetTransform().quaRotLocal).y);

	objects_[index]->Init(rotPos);
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
			AsoUtility::IsHitSpheres(
				dispenser->GetPos(), dispenser->GetSphereRad(),
				objects_[i]->GetSpherePos(), objects_[i]->GetSphereRad()))
		{
			//�ݒu����Ă���J�b�v�ɕX������
			if (auto iceCup = dynamic_cast<ItemObject*>(objects_[i].get()))
			{
				iceCup->PouredIce();

				//�X�𐶐����Ǐ]������
				objects_.emplace_back(std::make_unique<FollowingObject>("Ice",5.0f, player_, *objects_[i]));
				objects_.back()->Init(AsoUtility::VECTOR_ZERO);
				objects_.back()->Update();
				break;
			}
		}
	}
}

void StageManager::LidFollowCup(void)
{
	//bool isCreate = false;
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
				objects_.emplace_back(std::make_unique<FollowingObject>(lidType, 5.0f, player_, *objects_[i]));
				objects_.back()->Init(AsoUtility::VECTOR_ZERO);
				objects_.back()->Update();
				//isCreate = true;
				break;
			}
		}
		//if (isCreate)break;
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
			AsoUtility::IsHitSpheres(pSphere.GetPos(), pSphere.GetRadius(),
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