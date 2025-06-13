#include <DxLib.h>
#include "../Common/DebugDrawFormat.h"
#include "../Utility/AsoUtility.h"
#include "../Libs/ImGui/imgui.h"
#include "../Manager/Generic/ResourceManager.h"
#include "../Manager/Generic/InputManager.h"
#include "../Player.h"
#include "../Common/Sphere.h"
#include "StageObjectLibrary.h"
#include "StageObject.h"
#include "StageObject/ItemObject.h"
#include "StageObject/RackObject.h"
#include "StageObject/Table.h"
#include "StageObject/Machine.h"
#include "StageObject/CupLidRack.h"
#include "StageObject/CupLid.h"
#include "StageObject/DustBox.h"
#include "StageObject/IceDispenser.h"	
#include "StageManager.h"

#pragma region ����
//�N���X����������̂Ō��炷���߂ɃJ�e�S�����������N���X�ɂ�����
//��FHotCup,HotCoffee,Machine,IceDisoenser��ItemObject�Ƃ�MachineObject�Ƃ�

//�J�e�S�����������N���X�ɂ����json�f�[�^���������邩������
#pragma endregion


namespace {
	const std::string TABLE = "Table";		//�e�[�u��
	const std::string COUNTER = "Counter";	//�J�E���^�[
	const std::string HOT_CUP = "Hot_Cup";	//�z�b�g�p�J�b�v
	const std::string ICE_CUP = "Ice_Cup";	//�A�C�X�p�J�b�v
	const std::string HOT_CUP_RACK = "Cup_Hot_Rack";		//�z�b�g�p���b�N
	const std::string ICE_CUP_RACK = "Cup_Ice_Rack";		//�A�C�X�p���b�N
	const std::string CHOCO_SWEETSRACK = "Sweets_Choco_Rack";		//�`���R�X�C�[�c�p���b�N
	const std::string BERRY_SWEETSRACK = "Sweets_Strawberry_Rack";	//�x���[�X�C�[�c�p���b�N
	const std::string CHOCO_SWEETS = "Sweets_Choco";		//�`���R�X�C�[�c
	const std::string BERRY_SWEETS = "Sweets_Strawberry";	//�x���[�X�C�[�c
	const std::string CUP_WITH_ICE = "Cup_With_Ice";		//�A�C�X�p�J�b�v
	const std::string HOT_COFFEE = "Hot_Coffee";			//�z�b�g�R�[�q�[
	const std::string ICE_COFFEE = "Ice_Coffee";			//�A�C�X�R�[�q�[
	const std::string COFFEE_MACHINE = "Coffee_Machine";	//�R�[�q�[�}�V��
	const std::string CUP_LID_RACK = "Cup_Lid_Rack";		//�W�̃��b�N
	const std::string CUP_LID = "Cup_Lid";					//�W
	const std::string ICE_DISPENSER = "Ice_Dispenser";		//���X�@
	const std::string DUST_BOX = "Dust_Box";				//�S�~��
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

	//�X�V����
	updateFunc_.emplace(MODE::GAME_3D, std::bind(&StageManager::Update3DGame, this));
	updateFunc_.emplace(MODE::MACHINE_2D, std::bind(&StageManager::UpdateMachine2D, this));
	updateFunc_.emplace(MODE::ICE_2D, std::bind(&StageManager::UpdateIce2D, this));
	updateFunc_.emplace(MODE::LIDRACK_2D, std::bind(&StageManager::UpdateLidRack2D, this));

	//�`�揈��
	drawFunc_.emplace(MODE::GAME_3D, std::bind(&StageManager::Draw3DGame, this));
	drawFunc_.emplace(MODE::MACHINE_2D, std::bind(&StageManager::DrawMachine2D, this));
	drawFunc_.emplace(MODE::ICE_2D, std::bind(&StageManager::DrawIce2D, this));
	drawFunc_.emplace(MODE::LIDRACK_2D, std::bind(&StageManager::DrawLidRack2D, this));

	ChangeMode(MODE::GAME_3D); // �������[�h��3D�Q�[���ɐݒ�
}

StageManager::~StageManager(void)
{
	isServedItems_.clear();
}

void StageManager::Init(void)
{
	//���f������̊�{���
	transform_.SetModel(
		ResourceManager::GetInstance().LoadModelDuplicate(
			ResourceManager::SRC::STAGE));
	transform_.scl = AsoUtility::VECTOR_ONE;
	transform_.pos = AsoUtility::VECTOR_ZERO;
	transform_.quaRot = Quaternion::Euler(
		0.0f,
		AsoUtility::Deg2RadF(0.0f),
		0.0f
	);

	transform_.quaRotLocal = Quaternion();
	transform_.MakeCollider(Collider::TYPE::STAGE);
	transform_.Update();

	//���̃e�[�u���Q(��O)
	for (int x = 0; x < TABLE_ROW_FRONT_NUM; x++)
	{
		VECTOR firstPos = TABLE_POS_BACK;
		firstPos.x += (x * TABLE_WIDTH);
		tables_.emplace_back(std::make_unique<Table>(TABLE, TABLE_WIDTH, 76.0f, 60.0f, player_,objects_));
		tables_.back()->Init();
		tables_.back()->SetPos(firstPos);
	}

	//�c�̃e�[�u���Q
	for (int z = TABLE_ROW_FRONT_NUM; z < TABLE_ROW_FRONT_NUM + TABLE_COLUMN_NUM; z++)
	{
		VECTOR firstPos = COLUMN_TABLE_POS;
		firstPos.z += ((z - TABLE_ROW_FRONT_NUM) * TABLE_WIDTH);
		tables_.emplace_back(std::make_unique<Table>(TABLE, 60.0f, 76.0f, TABLE_WIDTH, player_,objects_));
		tables_.back()->Init();
		tables_.back()->SetPos(firstPos);
	}

	//���̃e�[�u���Q(����)
	for (int x = 0; x < TABLE_ROW_BACK_NUM; x++)
	{
		VECTOR firstPos = TABLE_POS_FRONT;
		firstPos.x += (x * TABLE_WIDTH);
		tables_.emplace_back(std::make_unique<Table>(TABLE, TABLE_WIDTH, 76.0f, 60.0f, player_, objects_));
		tables_.back()->Init();
		tables_.back()->SetPos(firstPos);
	}

	//�J�E���^�[�p�e�[�u��
	counter_ = std::make_unique<Table>(COUNTER, TABLE_WIDTH, 76.0f, 60.0f, player_, objects_);
	counter_->Init();
	counter_->SetPos(COUNTER_POS);

	//�z�b�g�p�J�b�v�̃��b�N
	objects_.emplace_back(std::make_unique<RackObject>(HOT_CUP_RACK, 60.0f, 20.0f, 60.0f, player_));
	objects_.back()->Init();
	objects_.back()->SetPos(tables_[TABLE_ROW_FRONT_NUM + TABLE_COLUMN_NUM - 1]->GetTopCenter());
	
	//�A�C�X�p�J�b�v�̃��b�N
	objects_.emplace_back(std::make_unique<RackObject>(ICE_CUP_RACK, 60.0f, 20.0f, 60.0f, player_));
	objects_.back()->Init();
	objects_.back()->SetPos(tables_[TABLE_COLUMN_NUM]->GetTopCenter());

	//�`���R�X�C�[�c�p�̃��b�N
	objects_.emplace_back(std::make_unique<RackObject>(CHOCO_SWEETSRACK, 60.0f, 20.0f, 60.0f, player_));
	objects_.back()->Init();
	objects_.back()->SetPos(tables_[tables_.size() - 2]->GetTopCenter());
	
	//�x���[�X�C�[�c�p�̃��b�N
	objects_.emplace_back(std::make_unique<RackObject>(BERRY_SWEETSRACK, 60.0f, 20.0f, 60.0f, player_));
	objects_.back()->Init();
	objects_.back()->SetPos(tables_.back()->GetTopCenter());
	
	//�J�b�v�p�̊W
	objects_.emplace_back(std::make_unique<CupLidRack>(CUP_LID_RACK, 60.0f, 20.0f, 60.0f, player_,objects_));
	objects_.back()->Init();
	objects_.back()->SetPos(tables_[2]->GetTopCenter());

	//�R�[�q�[�}�V��
	objects_.emplace_back(std::make_unique<Machine>(COFFEE_MACHINE, 50.0f, 60.0f, 75.0f,
		 player_,objects_));
	objects_.back()->Init();
	VECTOR pos = AsoUtility::VECTOR_ZERO;
	pos = tables_.front()->GetTopCenter();
	objects_.back()->SetPos(tables_[5]->GetTopCenter());

	//�A�C�X�f�B�X�y���T�[
	objects_.emplace_back(std::make_unique<IceDispenser>(ICE_DISPENSER, 50.0f, 75.0f, 60.0f,
		 player_,objects_));
	objects_.back()->Init();
	objects_.back()->SetPos(tables_[4]->GetTopCenter());

	//�S�~��
	objects_.emplace_back(std::make_unique<DustBox>(DUST_BOX, 50.0f, 75.0f, 60.0f,
		 player_,objects_));
	objects_.back()->Init();
	objects_.back()->SetPos(DUST_BOX_POS);

#ifdef _DEBUG

	//�J�E���^�[�O�̓����蔻��p�̋���
	sphereTran_.Update();

	sphereTran_.scl = AsoUtility::VECTOR_ONE;
	sphereTran_.pos = { 221.0f, 0.0f, 271.0f };
	sphereTran_.quaRot = Quaternion();
	sphereTran_.quaRotLocal =
		Quaternion::Euler({ 0.0f, AsoUtility::Deg2RadF(180.0f), 0.0f });

#endif // _DEBUG
}

void StageManager::Update(void)
{
	//�X�V�X�e�b�v
	modeUpdate_();

	auto& ins = InputManager::GetInstance();
	if(ins.IsTrgDown(KEY_INPUT_Q))
	{
		ChangeMode(MODE::ICE_2D);
	}
	if (ins.IsTrgDown(KEY_INPUT_E))
	{
		ChangeMode(MODE::GAME_3D);
	}

#ifdef _DEBUG

	//ImGui�̑�����s��
	//UpdateDebugImGui();

#endif // _DEBUG
}

void StageManager::Draw(void)
{
	modeDraw_(); // ���݂̃��[�h�ɉ������`��֐����Ăяo��

	//���f���̕`��
	MV1DrawModel(transform_.modelId);

	for (const auto& table : tables_)
	{
		table->Draw();
	}

	counter_->Draw();

	for (const auto& obj : objects_)
	{
		obj->Draw();
	}

#ifdef _DEBUG
	DrawDebug();
#endif // _DEBUG
}

void StageManager::SetCurrentOrder(const Order::OrderData& order)
{
	//���������̔z����m�ۂ��A�����������q�̒������e���󂯎��
	size_t size = static_cast<size_t>(order.num_);
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

void StageManager::SurveItem(StageObject& obj)
{
	// ���i����OrderData�ɕϊ�
	Order::OrderData data;
	// ��: obj��ID���Ԃ���OrderData���Z�b�g
	if (obj.GetObjectId() == HOT_COFFEE) 
	{
		servedItems_.drink_ = Order::DRINK::HOT;
		servedItems_.lid_ = obj.IsLidOn();
		isServedItems_.front() = true;
	}
	else if (obj.GetObjectId() == ICE_COFFEE) 
	{
		servedItems_.drink_ = Order::DRINK::ICE;
		servedItems_.lid_ = obj.IsLidOn();
		isServedItems_.front() = true;
	}

	if (obj.GetObjectId() == CHOCO_SWEETS)
	{
		servedItems_.sweets_ = Order::SWEETS::CHOCO;
		isServedItems_.back() = true;
	}
	else if (obj.GetObjectId() == BERRY_SWEETS)
	{
		servedItems_.sweets_ = Order::SWEETS::STRAWBERRY;
		isServedItems_.back() = true;
	}

	DeleteSurvedItem(); // �񋟌�̓I�u�W�F�N�g���폜

	// ������������������
	if (IsOrderCompleted()) 
	{
		isServed_ = true;
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
			it = objects_.erase(it); //erase�͎��̗v�f�̃C�e���[�^��Ԃ�
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
		if (!player_.GetIsHolding() && obj->GetIsCarryable() &&
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
				bool isAccepted = std::find(items.begin(), items.end(), obj->GetObjectId()) != items.end();
				if (!isAccepted)continue;	//���݂��Ȃ������珈�����Ȃ�

				obj->ItemPlaced(counter_->GetTopCenter());
			}

			for (const auto& table : tables_)
			{
				//�ݒu�\�ȃe�[�u���̏�ɃA�C�e����ݒu���鏈��
				if (table->GetIsPlaceable() &&
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
	for (const auto& obj : objects_)
	{
		if (obj->GetObjectId() != COFFEE_MACHINE)continue;

		//�����Ă���A�C�e�����}�V���ɐݒu���鏈��
		if (player_.GetIsHolding() &&
			AsoUtility::IsHitSpheres(pSphere.GetPos(), pSphere.GetRadius(),
				obj->GetSpherePos(), obj->GetSphereRad()))
		{
 			obj->Interact(player_.GetHoldItem());
		}

		//if(obj->GetMachineState() == StageObject::MACHINE_STATE::ACTIVE)
		//{
		//	ChangeMode(MODE::MACHINE_2D); // �}�V�����[�h�ɐ؂�ւ�
		//}

		//�ݒu���Ĉ�莞�Ԍo������R�[�q�[���o�͂���
		if (obj->GetObjectId() == COFFEE_MACHINE &&
			obj->GetInteractTime() <= 0.0f)
		{
			MakeCoffee();
			break;
		}
	}

	//�f�B�X�y���T�[�ƃJ�b�v�̏���
	for (const auto& obj : objects_)
	{
		if (obj->GetObjectId() != ICE_DISPENSER)continue;

		//�����Ă���A�C�e�����}�V���ɐݒu���鏈��
		if (player_.GetIsHolding() &&
			AsoUtility::IsHitSpheres(pSphere.GetPos(), pSphere.GetRadius(),
				obj->GetSpherePos(), obj->GetSphereRad()))
		{
			obj->Interact(player_.GetHoldItem());
		}

		//�ݒu���Ĉ�莞�Ԍo������X����J�b�v���o�͂���
		if (obj->GetInteractTime() <= 0.0f)
		{
			DispenseIce2Cup();
			break;
		}
	}
}

void StageManager::LidRackInteract(void)
{
	auto& pSphere = player_.GetSphere();
	//�R�[�q�[�ƊW�̏���
	for (const auto& obj : objects_)
	{
		if (obj->GetObjectId() != CUP_LID_RACK)continue;

		//�����Ă���R�[�q�[�ɊW�����鏈��
		if (player_.GetIsHolding() &&
			AsoUtility::IsHitSpheres(pSphere.GetPos(), pSphere.GetRadius(),
				obj->GetSpherePos(), obj->GetSphereRad()))
		{
			obj->Interact(player_.GetHoldItem());
		}
		else
		{
			//����O�̏ꍇ�͏����l�ɖ߂�
			obj->IsNotActioned();
			obj->SetInteractTime(3.0f);
		}

		//�C���^���N�g�������Ĉ�莞�Ԍo������W������
		if (obj->GetInteractTime() <= 0.0f)
		{
			LidFollowCup();
			break;
		}
	}
}

void StageManager::MakeCoffee(void)
{
	for (size_t i = 0; i < objects_.size(); ++i)
	{
		//�z�b�g�p�J�b�v�ȊO�̃I�u�W�F�N�g�͔��肵�Ȃ�
		if (objects_[i]->GetObjectId() != HOT_CUP &&
			objects_[i]->GetObjectId() != CUP_WITH_ICE) continue;

		for (const auto& machine : objects_)
		{
			//�}�V���̔��肾����������
			if (machine->GetObjectId() != COFFEE_MACHINE)continue;

			//�}�V���̋��̂Ɛݒu����Ă���J�b�v������������
			if (objects_[i]->GetItemState() == StageObject::ITEM_STATE::PLACED &&
				AsoUtility::IsHitSpheres(machine->GetPos(), machine->GetSphereRad(),
					objects_[i]->GetSpherePos(), objects_[i]->GetSphereRad()))
			{
				if(objects_[i]->GetObjectId() == HOT_CUP)
				{
					MakeHotCoffee(i);
					return; // �z�b�g�R�[�q�[��������珈�����I��
				}
				else {
					MakeIceCoffee(i);
					return; // �A�C�X�R�[�q�[��������珈�����I��
				}
			}
		}
	}
}

void StageManager::MakeHotCoffee(int i)
{
	//�ݒu����Ă���J�b�v���R�[�q�[�ɏ㏑������
	objects_[i] = std::make_unique<ItemObject>(HOT_COFFEE, 20.0f, 30.0f, 20.0f, player_);
	objects_[i]->Init();
	objects_[i]->ChangeItemState(StageObject::ITEM_STATE::PLACED);
	objects_[i]->SetPos(tables_[5]->GetTopCenter());
}

void StageManager::MakeIceCoffee(int i)
{
	//�ݒu����Ă���J�b�v���R�[�q�[�ɏ㏑������
	objects_[i] = std::make_unique<ItemObject>(ICE_COFFEE, 20.0f, 30.0f, 20.0f, player_);
	objects_[i]->Init();
	objects_[i]->ChangeItemState(StageObject::ITEM_STATE::PLACED);
	objects_[i]->SetPos(tables_[5]->GetTopCenter());
}

void StageManager::DispenseIce2Cup(void)
{
	for (size_t i = 0; i < objects_.size(); ++i)
	{
		//�A�C�X�p�J�b�v�ȊO�̃I�u�W�F�N�g�͔��肵�Ȃ�
		if (objects_[i]->GetObjectId() != ICE_CUP) continue;

		for (const auto& machine : objects_)
		{
			//�f�B�X�y���T�[�̔��肾����������
			if (machine->GetObjectId() != ICE_DISPENSER)continue;

			//�}�V���̋��̂Ɛݒu����Ă���J�b�v������������
			if (objects_[i]->GetItemState() == StageObject::ITEM_STATE::PLACED &&
				AsoUtility::IsHitSpheres(machine->GetPos(), machine->GetSphereRad(),
					objects_[i]->GetSpherePos(), objects_[i]->GetSphereRad()))
			{
				//�ݒu����Ă���J�b�v�ɕX������
				if (auto iceCup = dynamic_cast<ItemObject*>(objects_[i].get()))
				{
					iceCup->PouredIce();
				}
			}
		}
	}
}

void StageManager::LidFollowCup(void)
{
	bool isCreate = false;
	for (size_t i = 0; i < objects_.size(); ++i)
	{
		//�R�[�q�[�ȊO�̃I�u�W�F�N�g�͔��肵�Ȃ�
		if (objects_[i]->GetObjectId() != HOT_COFFEE &&
			objects_[i]->GetObjectId() != ICE_COFFEE) continue;

		//���ɊW������Ă���J�b�v�͔��肵�Ȃ�
		if (objects_[i]->IsLidOn())continue;

		for (const auto& lid : objects_)
		{
			//�W�̔��肾����������
			if (lid->GetObjectId() != CUP_LID_RACK)continue;

			//�W�̋��̂Ɠ������Ă���J�b�v������������
			if (AsoUtility::IsHitSpheres(lid->GetPos(), lid->GetSphereRad(),
				objects_[i]->GetSpherePos(), objects_[i]->GetSphereRad()))
			{
				//�R�[�q�[�I�u�W�F�N�g�ɊW������
				objects_[i]->PutOnTheLid();

				//�W�𐶐����Ǐ]������
				objects_.emplace_back(std::make_unique<CupLid>(CUP_LID, 23.0f, 5.0f, 23.0f, player_, *objects_[i]));
				objects_.back()->Init();
				objects_.back()->Update();
				isCreate = true;
				break;
			}
		}
		if (isCreate)break;
	}
}

void StageManager::DustBoxInteract(void)
{
	auto& ins = InputManager::GetInstance();
	auto& pSphere = player_.GetSphere();

	// �S�~���̏���
	for (const auto& obj : objects_)
	{
		if (obj->GetObjectId() != DUST_BOX) continue;

		// �v���C���[�������Ă���A�C�e�����S�~���ɋ߂Â��Ă���ꍇ
		if (player_.GetIsHolding() &&
			AsoUtility::IsHitSpheres(pSphere.GetPos(), pSphere.GetRadius(),
				obj->GetSpherePos(), obj->GetSphereRad()))
		{
			if(ins.IsInputTriggered("Interact"))
			{
				//�S�~���ɃA�C�e�����̂Ă鏈��
				//DiscardHoldObject();
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


void StageManager::ChangeMode(MODE mode)
{
	mode_ = mode;

	// �֐��|�C���^�؂�ւ�
	modeUpdate_ = updateFunc_[mode_];
	modeDraw_ = drawFunc_[mode_];
}

void StageManager::ChangeMode3DGame(void)
{
	// TODO: ������3D�Q�[�����[�h�؂�ւ�����������
}

void StageManager::ChangeModeMachine2D(void)
{
	// TODO: ������2D�}�V�����[�h�؂�ւ�����������
}

void StageManager::ChangeModeIce2D(void)
{
	// TODO: ������2D���X�@���[�h�؂�ւ�����������
}

void StageManager::ChangeModeLidRack2D(void)
{
	// TODO: ������2D���b�h���b�N���[�h�؂�ւ�����������
}

void StageManager::Update3DGame(void)
{
	auto& ins = InputManager::GetInstance();

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

	//���b�N����J�b�v�����o������
	for (const auto& obj : objects_)
	{
		//�v���C���[�����������Ă��Ȃ��Ƃ��̏���
		if (!player_.GetIsHolding() && obj->GetIsInteractable() &&
			AsoUtility::IsHitSpheres(pSphere.GetPos(), pSphere.GetRadius(),
				obj->GetSpherePos(), obj->GetSphereRad()))
		{
			obj->PickUp(obj->GetObjectId(), objects_);
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
	sphereTran_.Update();
}

void StageManager::UpdateMachine2D(void)
{
	// TODO: 2D�}�V�����[�h�̍X�V����
}

void StageManager::UpdateIce2D(void)
{
	// TODO: 2D���X�@���[�h�̍X�V����
}

void StageManager::UpdateLidRack2D(void)
{
	// TODO: 2D���b�h���b�N���[�h�̍X�V����
}

void StageManager::Draw3DGame(void)
{
	// TODO: 3D�Q�[�����[�h�̕`�揈��
}

void StageManager::DrawMachine2D(void)
{
	// TODO: 2D�}�V�����[�h�̕`�揈��
}

void StageManager::DrawIce2D(void)
{
	// TODO: 2D���X�@���[�h�̕`�揈��
}

void StageManager::DrawLidRack2D(void) {
	// TODO: 2D���b�h���b�N���[�h�̕`�揈��
}

void StageManager::DrawDebug(void)
{
	//DrawSphere3D(objects_[3]->GetSpherePos(),
	//	objects_[3]->GetSphereRad(), 8, 0xff0000, 0xff0000, true);

	int line = 8;	//�s
	int lineHeight = 30;	//�s
	DebugDrawFormat::FormatString(L"item : %s", StringUtility::StringToWstring(player_.GetHoldItem()).c_str(), line, lineHeight);
	DebugDrawFormat::FormatString(L"hold : %d", player_.GetIsHolding(), line, lineHeight);
	DebugDrawFormat::FormatString(L"mode : %d", mode_, line, lineHeight);

	DebugDrawFormat::FormatString(L"currentD,S : %d,%d",
			currentOrder_.drink_, currentOrder_.sweets_, line, lineHeight);

	DebugDrawFormat::FormatString(L"surevdD,S : %d,%d",
		servedItems_.drink_, servedItems_.sweets_, line, lineHeight);

	DebugDrawFormat::FormatString(L"boolSize : %d",
			isServedItems_.size(), line, lineHeight);
		
	//size_t size = objects_.size();
	////�W�������m�F�p
	//DebugDrawFormat::FormatString(L"end - 2 : %s",
	//	StringUtility::StringToWstring(objects_[size - 3]->GetObjectId()).c_str(), line, lineHeight);
	//
	//DebugDrawFormat::FormatString(L"end - 1 : %s",
	//	StringUtility::StringToWstring(objects_[size - 2]->GetObjectId()).c_str(), line, lineHeight);

	//DebugDrawFormat::FormatString(L"end : %s",
	//	StringUtility::StringToWstring(objects_.back()->GetObjectId()).c_str(), line, lineHeight);

	//DebugDrawFormat::FormatString(L"surveD : %d",
	//	surveDrink_, line, lineHeight);
	//
	//DebugDrawFormat::FormatString(L"surveS : %d",
	//	surveSweets_, line, lineHeight);
	//
	//DebugDrawFormat::FormatString(L"surveL : %d",
	//	surveDrinkLid_, line, lineHeight);
	//
	//DebugDrawFormat::FormatString(L"surve : %d",
	//	isSurved_, line, lineHeight);

	//for (const auto& obj : objects_)
	//{
	//	DebugDrawFormat::FormatString(L"objActioned : %d",
	//		obj->IsActioned() , line, lineHeight);
	//}

	for (size_t i = 0; i < objects_.size(); ++i)
	{
		//�R�[�q�[�ȊO�̃I�u�W�F�N�g�͔��肵�Ȃ�
		if (objects_[i]->GetObjectId() != HOT_COFFEE &&
			objects_[i]->GetObjectId() != ICE_COFFEE) continue;

		DebugDrawFormat::FormatString(L"coffee%d.lid  : %d", i,
			objects_[i]->IsLidOn(), line, lineHeight);
	}

	//for (int i = 0; i < tables_.size(); i++)
	//{
	//	DebugDrawFormat::FormatString(L"table%d.placeable  : %d", i,
	//		tables_[i]->GetIsPlaceable(), line, lineHeight);
	//}

	//�e�[�u���ԍ���\��
	for (int i = 0; i < TABLE_COLUMN_NUM + TABLE_ROW_FRONT_NUM - 1; i++)
	{
		VECTOR screenPos = ConvWorldPosToScreenPos(tables_[i]->GetTransform().pos);
		// �ϊ�����
		DrawFormatString(static_cast<int>(screenPos.x) - 25, static_cast<int>(screenPos.y) - 50, GetColor(255, 255, 255),
			L"%s : %d",
			StringUtility::StringToWstring(tables_[i]->GetObjectId().c_str()).c_str(), i);
	}

	for (const auto& obj : objects_)
	{
		VECTOR screenPos = ConvWorldPosToScreenPos(obj->GetTransform().pos);
		// �ϊ�����
		DrawFormatString(static_cast<int>(screenPos.x) - 25, static_cast<int>(screenPos.y) - 50, GetColor(255, 255, 255),
			L"%s",
			StringUtility::StringToWstring(obj->GetObjectId().c_str()).c_str());
	}
}

void StageManager::UpdateDebugImGui(void)
{
	//�E�B���h�E�^�C�g��&�J�n����
	ImGui::Begin("Player:Circle");
	//�傫��
	ImGui::Text("scale");
	ImGui::InputFloat("SclX", &sphereTran_.scl.x);
	ImGui::InputFloat("SclY", &sphereTran_.scl.y);
	ImGui::InputFloat("SclZ", &sphereTran_.scl.z);

	//�p�x
	VECTOR rotDeg = VECTOR();
	rotDeg.x = AsoUtility::Rad2DegF(sphereTran_.rot.x);
	rotDeg.y = AsoUtility::Rad2DegF(sphereTran_.rot.y);
	rotDeg.z = AsoUtility::Rad2DegF(sphereTran_.rot.z);
	ImGui::Text("angle(deg)");
	ImGui::SliderFloat("RotX", &rotDeg.x, 0.0f, 360.0f);
	ImGui::SliderFloat("RotY", &rotDeg.y, 0.0f, 360.0f);
	ImGui::SliderFloat("RotZ", &rotDeg.z, 0.0f, 360.0f);
	sphereTran_.rot.x = AsoUtility::Deg2RadF(rotDeg.x);
	sphereTran_.rot.y = AsoUtility::Deg2RadF(rotDeg.y);
	sphereTran_.rot.z = AsoUtility::Deg2RadF(rotDeg.z);

	//�ʒu
	ImGui::Text("position");
	//�\���̂̐擪�|�C���^��n���Axyz�ƘA�������������z�u�փA�N�Z�X
	ImGui::InputFloat3("Pos", &sphereTran_.pos.x);
	ImGui::SliderFloat("PosX", &sphereTran_.pos.x, 0.0f, 360.0f);
	ImGui::SliderFloat("PosY", &sphereTran_.pos.y, 0.0f, 360.0f);
	ImGui::SliderFloat("PosZ", &sphereTran_.pos.z, 0.0f, 1000.0f);

	//�I������
	ImGui::End();
}
