#include "../Utility/AsoUtility.h"
#include "../Manager/Generic/SceneManager.h"
#include "../Manager/Generic/InputManager.h"
#include "../Manager/Generic/ResourceManager.h"
#include "../Object/Common/Sphere.h"
#include "../Object/Player.h"
#include "../Object/Stage/StageManager.h"
#include "../../UI/GaugeUI.h"
#include "../../UI/IconUI.h"
#include "../../UI/UIManager.h"
#include "IceDispenser.h"

IceDispenser::IceDispenser(const std::string objId,
	const float height, Player& player,
	std::vector<std::unique_ptr<StageObject>>& object) :
	StageObject(objId, height, player), objects_(object)
{
}

void IceDispenser::Init(VECTOR pos, float rotY, VECTOR scale)
{
	StageObject::Init(pos, rotY,scale);

	iconUI_ = std::make_unique<IconUI>(VGet(0.0f, 160.0f, 0.0f),
		transform_.pos, ResourceManager::SRC::ICE_IN);
	iconUI_->Init();
	iconUI_->SetActive(false);
	UIManager::GetInstance().AddIconUI(iconUI_.get());

	gaugeUI_ = std::make_unique<GaugeUI>(false, ICE_PRODUCES_TIME);
	gaugeUI_->Init();
	VECTOR uiPos = transform_.pos;
	uiPos.y += 160.0f;	//UI�̈ʒu�𒲐�
	gaugeUI_->SetPos(uiPos); // UI�̈ʒu��ݒ�
	UIManager::GetInstance().AddGaugeUI(gaugeUI_.get());
}

void IceDispenser::Draw(void)
{
	StageObject::Draw();
}

void IceDispenser::Interact(const std::string& objId)
{
	if (GetMachineState() == MACHINE_STATE::ACTIVE)return;	//�A�N�e�B�u��Ԃ������珈�����Ȃ�

	auto& items = param_.acceptedItems_;
	//objId���C���^���N�g�Ώە��ɑ��݂��邩�ǂ���
	bool isAccepted = std::find(items.begin(), items.end(), objId) != items.end();
	if (!isAccepted)return;	//���݂��Ȃ������珈�����Ȃ�

	//���Ƀ}�V�����ɃJ�b�v�܂��̓R�[�q�[������ꍇ�͉������Ȃ�
	for (const auto& obj : objects_)
	{
		//�A�C�X�J�b�v�܂��̓A�C�X�R�[�q�[
		if ((obj->GetParam().id_ == ICE_CUP || obj->GetParam().id_ == CUP_WITH_ICE) &&
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
		//�A�C�X�p�J�b�v�ȊO�̃I�u�W�F�N�g�͔��肵�Ȃ�
		if (obj->GetParam().id_ != ICE_CUP) continue;

		if (AsoUtility::IsHitSpheres(GetSpherePos(), GetSphereRad(),
			obj->GetSpherePos(), obj->GetSphereRad()))
		{
			iconUI_->SetActive(true);
			//�X�y�[�X�L�[�����Ń}�V���̏ꏊ�ɃJ�b�v��u��(�Ƃ肠����)
			if (player_.GetHoldItem() == items.front().c_str() &&
				ins.IsInputTriggered("Interact"))
			{
				//�}�V���̏�ɏ��悤�ɃJ�b�v��z�u����
				VECTOR cupPos = GetTransform().pos;
				cupPos = VAdd(cupPos, { 0.0f,MACHINE_OFSET_Y ,DISPENSER_OFSET_Z });

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

void IceDispenser::UpdateInActive(void)
{
	SetInteractTime(ICE_PRODUCES_TIME);
	iconUI_->SetActive(false);
	//�}�V���̓����蔻�����PLACED��Ԃ̃J�b�v�����݂��邩�`�F�b�N
	bool hasPlacedCup = false;
	for (const auto& obj : objects_)
	{
		if (obj->GetParam().id_ != CUP_WITH_ICE)continue;

		if (AsoUtility::IsHitSpheres(obj->GetSpherePos(), obj->GetSphereRad(),
			GetSpherePos(), GetSphereRad()) &&
			obj->GetItemState() == ITEM_STATE::PLACED)
		{
			hasPlacedCup = true;
			break;
		}
	}

	if (!hasPlacedCup)
	{
		gaugeUI_->Reset();
	}
}

void IceDispenser::UpdateActive(void)
{
	param_.interactTime_ -= SceneManager::GetInstance().GetDeltaTime();
	iconUI_->SetActive(false);
	gaugeUI_->Update();

	//�}�V���̓����蔻�����PLACED��Ԃ̃J�b�v�����݂��邩�`�F�b�N
	bool hasPlacedCup = false;
	for (const auto& obj : objects_)
	{
		if (obj->GetParam().id_ != ICE_CUP)continue;

		if (AsoUtility::IsHitSpheres(obj->GetSpherePos(), obj->GetSphereRad(),
			GetSpherePos(), GetSphereRad()) &&
			obj->GetItemState() == ITEM_STATE::PLACED &&
			param_.interactTime_ >= 0.0f)
		{
			hasPlacedCup = true;
			break;
		}
	}

	//PLACED��Ԃ̃J�b�v���Ȃ���Δ�A�N�e�B�u�ɂ���
	if (!hasPlacedCup)
	{
		ChangeMachineState(MACHINE_STATE::INACTIVE);
		return;
	}
}
