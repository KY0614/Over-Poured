#include "../Utility/AsoUtility.h"
#include "../Manager/Generic/SceneManager.h"
#include "../Manager/Generic/InputManager.h"
#include "../Object/Common/Sphere.h"
#include "../Object/Player.h"
#include "../Object/Stage/StageManager.h"
#include "../../UI/IconUI.h"
#include "../../UI/GaugeUI.h"
#include "../../UI/UIManager.h"
#include "Machine.h"

Machine::Machine(const std::string objId,Player& player,
	std::vector<std::unique_ptr<StageObject>>& object) :
	StageObject(objId,player),objects_(object)
{
}

void Machine::Interact(const std::string& objId)
{
	if (GetMachineState() == MACHINE_STATE::ACTIVE)return;	//�A�N�e�B�u��Ԃ������珈�����Ȃ�

	auto& items = param_.acceptedItems_;
	//objId���C���^���N�g�Ώە��ɑ��݂��邩�ǂ���
	bool isAccepted = std::find(items.begin(), items.end(), objId) != items.end();
	if (!isAccepted)return;	//���݂��Ȃ������珈�����Ȃ�

	//���Ƀ}�V�����ɃJ�b�v�܂��̓R�[�q�[������ꍇ�͉������Ȃ�
	for (const auto& obj : objects_)
	{
		// �z�b�g�J�b�v�܂��̓z�b�g�R�[�q�[
		if ((obj->GetParam().id_ == HOT_CUP ||
			obj->GetParam().id_ == HOT_COFFEE ||
			obj->GetParam().id_ == CUP_WITH_ICE ||
			obj->GetParam().id_ == ICE_COFFEE) &&
			AsoUtility::IsHitSpheres(GetSpherePos(), GetSphereRad(),
				obj->GetSpherePos(), obj->GetSphereRad()) &&
			obj->GetItemState() == ITEM_STATE::PLACED)
		{
			// ���ɃZ�b�g�ς�
			return;
		}
	}

	auto& ins = InputManager::GetInstance();
	for (const auto& obj : objects_)
	{
		//�J�b�v�ȊO�̃I�u�W�F�N�g�͔��肵�Ȃ�
		if (obj->GetParam().id_ != HOT_CUP && obj->GetParam().id_ != CUP_WITH_ICE) continue;

		if (AsoUtility::IsHitSpheres(GetSpherePos(), GetSphereRad(),
			obj->GetSpherePos(), obj->GetSphereRad()))
		{
			iconUI_->SetActive(true);
			//�X�y�[�X�L�[�����Ń}�V���̏ꏊ�ɃJ�b�v��u��
			if ((player_.GetHoldItem() == items.front().c_str() ||
				player_.GetHoldItem() == items.back().c_str()) &&
				ins.IsInputTriggered("Interact"))
			{
				//�}�V���̏�ɏ��悤�ɃJ�b�v��z�u����
				VECTOR cupPos = GetTransform().pos;	
				cupPos = VAdd(cupPos, { 0.0f,MACHINE_OFSET_Y ,MACHINE_OFSET_Z });

				//�}�V���̉�]�ɍ��킹�ăJ�b�v�̈ʒu�𒲐�
				VECTOR rotPos = AsoUtility::RotXZPos(GetTransform().pos, cupPos,
					Quaternion::ToEuler(GetTransform().quaRotLocal).y);

				obj->ItemPlaced(rotPos);
				gaugeUI_->SetActive(true);
				ChangeMachineState(MACHINE_STATE::ACTIVE);
			}
		}
	}
}

void Machine::Init(VECTOR pos, float rotY, VECTOR scale)
{
	StageObject::Init(pos, rotY, scale);

	iconUI_ = std::make_unique<IconUI>(VGet(0.0f, UI_OFFSET_Y, 0.0f),
		transform_.pos, ResourceManager::SRC::BREW_COFFEE);
	iconUI_->Init();
	iconUI_->SetActive(false);
	UIManager::GetInstance().AddIconUI(iconUI_.get());
	
	hotIconUI_ = std::make_unique<IconUI>(VGet(0.0f, UI_OFFSET_Y, 0.0f),
		transform_.pos, ResourceManager::SRC::HOT_ICON);
	hotIconUI_->Init();
	hotIconUI_->SetActive(false);
	UIManager::GetInstance().AddIconUI(hotIconUI_.get());
		
	iceIconUI_ = std::make_unique<IconUI>(VGet(0.0f, UI_OFFSET_Y, 0.0f),
		transform_.pos, ResourceManager::SRC::ICE_ICON);
	iceIconUI_->Init();
	iceIconUI_->SetActive(false);
	UIManager::GetInstance().AddIconUI(iceIconUI_.get());

	gaugeUI_ = std::make_unique<GaugeUI>(false, COFFEE_PRODUCES_TIME);
	gaugeUI_->Init();
	VECTOR uiPos = transform_.pos;
	uiPos.y += UI_OFFSET_Y;	//UI�̈ʒu�𒲐�
	gaugeUI_->SetPos(uiPos); // UI�̈ʒu��ݒ�1
	UIManager::GetInstance().AddGaugeUI(gaugeUI_.get());
}

void Machine::UpdateInActive(void)
{
	hotIconUI_->SetActive(false);
	iceIconUI_->SetActive(false);
	iconUI_->SetActive(false);
	gaugeUI_->SetActive(false);
	SetInteractTime(COFFEE_PRODUCES_TIME);

	//�}�V���̓����蔻�����PLACED��Ԃ̃J�b�v�����݂��邩�`�F�b�N
	bool hasPlacedCup = false;
	for (const auto& obj : objects_)
	{
		if (obj->GetParam().id_ != HOT_COFFEE &&
			obj->GetParam().id_ != ICE_COFFEE)continue;

		if (AsoUtility::IsHitSpheres(obj->GetSpherePos(), obj->GetSphereRad(),
			GetSpherePos(), GetSphereRad()) &&
			obj->GetItemState() == ITEM_STATE::PLACED)
		{
			if (obj->GetParam().id_ == HOT_COFFEE)
			{
				hotIconUI_->SetActive(true);
			}
			else
			{
				iceIconUI_->SetActive(true);
			}
			hasPlacedCup = true;
			break;
		}
	}

	if(!hasPlacedCup)
	{
		gaugeUI_->Reset();
	}
}

void Machine::UpdateActive(void)
{
	iconUI_->SetActive(false);
	hotIconUI_->SetActive(false);
	iceIconUI_->SetActive(false);
	param_.interactTime_ -= SceneManager::GetInstance().GetDeltaTime();

	gaugeUI_->Update();

	//�}�V���̓����蔻�����PLACED��Ԃ̃J�b�v�����݂��邩�`�F�b�N
	bool hasPlacedCup = false;
	for (const auto& obj : objects_)
	{
		if (obj->GetParam().id_ != HOT_CUP &&
			obj->GetParam().id_ != CUP_WITH_ICE )continue;

		if (AsoUtility::IsHitSpheres(obj->GetSpherePos(), obj->GetSphereRad(),
			GetSpherePos(), GetSphereRad()) &&
			obj->GetItemState() == ITEM_STATE::PLACED)
		{
			hasPlacedCup = true;
			break;
		}
	}

	//PLACED��Ԃ̃J�b�v���Ȃ���Δ�A�N�e�B�u�ɂ���
	if (!hasPlacedCup || param_.interactTime_ <= 0.0f)
	{
		ChangeMachineState(MACHINE_STATE::INACTIVE);
		return;
	}
}

void Machine::Draw(void)
{
	StageObject::Draw();
}
