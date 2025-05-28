#include <DxLib.h>
#include <fstream>
#include"../Libs/nlohmann/json.hpp"
#include "../Common/DebugDrawFormat.h"
#include "../Utility/AsoUtility.h"
#include "../Libs/ImGui/imgui.h"
#include "../Manager/Generic/ResourceManager.h"
#include "../Manager/Generic/InputManager.h"
#include "../Player.h"
#include "../Common/Sphere.h"
#include "StageObjectLibrary.h"
#include "StageObject.h"
#include "StageObject/Table.h"
#include "StageObject/Machine.h"
#include "StageObject/HotCup.h"
#include "StageObject/IceCup.h"
#include "StageObject/HotCupRack.h"
#include "StageObject/HotCoffee.h"
#include "StageManager.h"

namespace {
	const std::string TABLE = "Table";		//�e�[�u��
	const std::string HOT_CUP = "Hot_Cup";	//�z�b�g�p�J�b�v
	const std::string ICE_CUP = "Ice_Cup";	//�A�C�X�p�J�b�v
	const std::string HOT_CUP_RACK = "Cup_Hot_Rack";	//�z�b�g�p���b�N
	const std::string CUP_WITH_ICE = "Cup_With_Ice";		//�A�C�X�p�J�b�v
	const std::string COFFEE_MACHINE = "Coffee_Machine";	//�R�[�q�[�}�V��
	const std::string LIB = "";		//
}


StageManager::StageManager(Vector2 mapSize, Player& player):player_(player)
{
	size_.height_ = mapSize.y;
	size_.width_ = mapSize.x;
}

StageManager::~StageManager(void)
{
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

	//���̃e�[�u���Q
	for (int x = 0; x < TABLE_NUM; x++)
	{
		VECTOR firstPos = TABLE_POS;
		firstPos.x += (x * TABLE_WIDTH);
		tables_.emplace_back(std::make_unique<Table>(TABLE, TABLE_WIDTH, 76.0f, 60.0f, player_,objects_));
		tables_.back()->Init();
		tables_.back()->SetPos(firstPos);
	}
	//�c�̃e�[�u���Q
	for (int z = TABLE_NUM; z < TABLE_NUM + TABLE_NUM - 1; z++)
	{
		VECTOR firstPos = COLUMN_TABLE_POS;
		firstPos.z += ((z - TABLE_NUM) * TABLE_WIDTH);
		tables_.emplace_back(std::make_unique<Table>(TABLE, 60.0f, 76.0f, TABLE_WIDTH, player_,objects_));
		tables_.back()->Init();
		tables_.back()->SetPos(firstPos);
	}

	//�z�b�g�p�J�b�v�̃��b�N
	objects_.emplace_back(std::make_unique<HotCupRack>(HOT_CUP_RACK, 60.0f, 20.0f, 60.0f, player_));
	objects_.back()->Init();
	objects_.back()->SetPos(tables_[1]->GetTopCenter());
	
	//�z�b�g�p�J�b�v
	objects_.emplace_back(std::make_unique<HotCup>(HOT_CUP, 40.0f, 30.0f, 40.0f, player_));
	StageObject& cupHotRef = *objects_.back();
	objects_.back()->Init();
	objects_.back()->SetPos(tables_.back()->GetTopCenter());

	//�A�C�X�p�J�b�v
	objects_.emplace_back(std::make_unique<IceCup>(ICE_CUP, 40.0f, 30.0f, 40.0f, player_));
	StageObject& cupIceRef = *objects_.back(); // �V�����ǉ����ꂽIceCup�ւ̎Q�Ƃ��擾
	objects_.back()->Init();
	objects_.back()->SetPos(tables_[tables_.size() - 2]->GetTopCenter());

	//�R�[�q�[�}�V��
	objects_.emplace_back(std::make_unique<Machine>(COFFEE_MACHINE, 75.0f, 60.0f, 50.0f,
		 player_));
	objects_.back()->Init();
	VECTOR pos = AsoUtility::VECTOR_ZERO;
	pos = tables_.front()->GetTopCenter();
	objects_.back()->SetPos(pos);
	
	////�z�b�g�p�J�b�v
	//cupH_ = std::make_unique<CupHot>("Cup_Hot", 40.0f, 30.0f, 40.0f);
	//cupH_->Init();
	//cupH_->SetPos(CUPHOT_POS);
	
	////�A�C�X�p�J�b�v
	//cupI_ = std::make_unique<IceCup>("Cup_Ice", 40.0f, 30.0f, 40.0f);
	//cupI_->Init();
	//cupI_->SetPos(CUPICE_POS);
	
	////�R�[�q�[�}�V��
	//machine_ = std::make_unique<Machine>("Coffee_Machine", 75.0f, 60.0f, 60.0f,*cupH_,*cupI_);
	//machine_->Init();
	//machine_->SetPos(MACHINE_POS);

	////�A�C�X�f�B�X�y���T�[(�X�������j
	//iceDispenser_ = std::make_unique<StageObject>("Ice_Dispenser", 60.0f, 20.0f, 60.0f);
	//iceDispenser_->Init();
	//iceDispenser_->SetPos(ICEDIS_POS);
	//	
	////�W��
	//libs_ = std::make_unique<StageObject>("Libs", 40.0f, 30.0f, 40.0f);
	//libs_->Init();
	//libs_->SetPos(LIBS_POS);
	//		
	////�S�~��
	//dustBox_ = std::make_unique<StageObject>("Dust_Box", 40.0f, 70.0f, 40.0f);
	//dustBox_->Init();
	//dustBox_->SetPos(DUSTBOX_POS);

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
	auto& ins = InputManager::GetInstance();

	//machine_->Update();
	//table_->Update();
	//cupH_->Update();
	//cupI_->Update();
	//iceDispenser_->Update();
	//libs_->Update();
	//dustBox_->Update();

	auto& pSphere = player_.GetSphere();

	for (const auto& obj : objects_)
	{
		obj->Update();
	}

	for (const auto& obj : tables_)
	{
		obj->Update();
	}

	//���b�N����J�b�v�����o������
	for (const auto& obj : objects_)
	{
		//�v���C���[�����������Ă��Ȃ��Ƃ��̏���
		if (!player_.GetIsHolding() && obj->IsInteractable() &&
			AsoUtility::IsHitSpheres(pSphere.GetPos(), pSphere.GetRadius(),
				obj->GetSpherePos(), obj->GetSphereRad()))
		{
			obj->PickUp(objects_);
			break;
		}
	}

	//�����^�щ\�ȃA�C�e���ɑ΂��Ă̏���
	for (const auto& obj : objects_)
	{

		//�v���C���[�����������Ă��Ȃ��Ƃ��̏���
		if (!player_.GetIsHolding() && obj->IsCarryable() &&
			AsoUtility::IsHitSpheres(pSphere.GetPos(), pSphere.GetRadius(),
				obj->GetSpherePos(), obj->GetSphereRad()))
		{
			obj->ItemCarry();
			break;
		}

		//�v���C���[���A�C�e���������Ă���Ƃ��̏���
		if (player_.GetIsHolding())
		{
			for (const auto& table : tables_)
			{
				//�ݒu�\�ȃe�[�u���̏�ɃA�C�e����ݒu���鏈��
				if(table->IsPlaceable() &&
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

	for (const auto& obj : objects_)
	{
		if (obj->GetObjectId() != COFFEE_MACHINE)continue;

		//�����Ă���A�C�e�����}�V���ɐݒu���鏈��
		if (obj->GetInteractType() == "machine" && player_.GetIsHolding()&&
			AsoUtility::IsHitSpheres(pSphere.GetPos(), pSphere.GetRadius(),
			obj->GetSpherePos(), obj->GetSphereRad()))
		{
			obj->Interact(player_.GetHoldItem(), objects_);
		}

		if (obj->GetInteractTime() <= 0.0f)
		{
			
		}
	}


	////�J�b�v�ƃ}�V���̋��̔���
	//if (AsoUtility::IsHitSpheres(cupH_->GetSpherePos(), 20.0f,
	//	machine_->GetSpherePos(), 35.0f))
	//{
	//	//�J�b�v���ݒu��Ԃł��v���C���[���}�V���̑O�ɂ���Ƃ���
	//	//R�L�[�����Ńz�b�g�R�[�q�[�쐬
	//	if (cupH_->GetState() == StageObject::STATE::PLACED &&
	//		AsoUtility::IsHitSpheres(pSphere.GetPos(), pSphere.GetRadius(),
	//			machine_->GetSpherePos(), 35.0f))
	//	{
	//		if (ins.IsTrgDown(KEY_INPUT_R))
	//		{
	//			//�z�b�g�R�[�q�[����
	//			cupH_ = std::make_unique<CupHot>("Hot_Coffee", 40.0f, 30.0f, 40.0f);
	//			cupH_->Init();
	//			cupH_->SetPos(MACHINE_POS);
	//		}
	//	}
	//}


#ifdef _DEBUG

	//ImGui�̑�����s��
	//UpdateDebugImGui();

#endif // _DEBUG

	transform_.Update();
	sphereTran_.Update();

}

void StageManager::Draw(void)
{
	//���f���̕`��

	MV1DrawModel(transform_.modelId);
	DrawSphere3D(sphereTran_.pos, 30, 8, 0xff0000, 0xff0000, false);

	//machine_->Draw();
	//table_->Draw();
	//cupH_->Draw();
	//cupI_->Draw();
	//iceDispenser_->Draw();
	//libs_->Draw();
	//dustBox_->Draw();
	for (const auto& table : tables_)
	{
		table->Draw();
	}

	for (const auto& obj : objects_)
	{
		obj->Draw();
	}

#ifdef _DEBUG

	int line = 8;	//�s
	int lineHeight = 30;	//�s
	DebugDrawFormat::FormatString(L"item : %s", StringUtility::StringToWstring(player_.GetHoldItem()).c_str(), line, lineHeight);
	DebugDrawFormat::FormatString(L"hold : %d", player_.GetIsHolding(), line, lineHeight);
	for (int i = 0; i < tables_.size(); i++)
	{
		DebugDrawFormat::FormatString(L"table%d.placeable  : %d", i,
			tables_[i]->IsPlaceable(), line, lineHeight);
	}

	for (int i = 0; i < TABLE_NUM + TABLE_NUM - 1; i++)
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

#endif // _DEBUG
}

void StageManager::MakeHotCoffee(void)
{
	player_.SetIsHoldiong(false);

	//auto* hot = FindValue<HotCup>(objects_);

	////�z�b�g�p�J�b�v
	//hot = std::make_unique<HotCoffee>("Hot_Coffee", 40.0f, 30.0f, 40.0f);
	//hot->Init();
	//hot->SetPos(MACHINE_POS);
}

template<typename Value>
inline Value* StageManager::FindValue(const std::vector<std::unique_ptr<StageObject>>& objects)
{
	for (const auto& obj : objects) {
		if (auto* object = dynamic_cast<Value*>(obj.get())) {
			return object;
		}
	}
	return nullptr;
}


bool StageManager::IsInBounds(int x, int y) const
{
	return (x >= 0 && x < size_.width_ && y >= 0 && y < size_.height_);
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
