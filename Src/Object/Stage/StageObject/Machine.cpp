#include "../Libs/ImGui/imgui.h"
#include "../Common/DebugDrawFormat.h"
#include "../Utility/AsoUtility.h"
#include "../Manager/Generic/SceneManager.h"
#include "../Manager/Generic/InputManager.h"
#include "../Object/Common/Sphere.h"
#include "../Object/Player.h"
#include "../Object/Stage/StageManager.h"
#include "Machine.h"

namespace {
	const std::string HOT_CUP = "Hot_Cup";	//�z�b�g�p�J�b�v
	const std::string CUP_WITH_ICE = "Cup_With_Ice";		//�A�C�X�p�J�b�v
	const std::string HOT_COFFEE = "Hot_Coffee";		//�z�b�g�R�[�q�[
	const std::string ICE_COFFEE = "Ice_Coffee";		//�A�C�X�R�[�q�[
}

Machine::Machine(const std::string objId, const float width,
	const float height, const float depth,Player& player,
	std::vector<std::unique_ptr<StageObject>>& object) :
	StageObject(objId, width, height, depth,player),objects_(object)
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
			//�X�y�[�X�L�[�����Ń}�V���̏ꏊ�ɃJ�b�v��u��
			if ((player_.GetHoldItem() == items.front().c_str() ||
				player_.GetHoldItem() == items.back().c_str()) &&
				ins.IsInputTriggered("Interact"))
			{
				//�}�V���̏�ɏ��悤�ɃJ�b�v��z�u����
				VECTOR cupPos = GetTransform().pos;	
				cupPos.y += MACHINE_OFSET_Y;	//������ɂ��炷
				cupPos.x += MACHINE_OFSET_X;	//�����E�ɂ��炷

				obj->ItemPlaced(cupPos);
				ChangeMachineState(MACHINE_STATE::ACTIVE);
			}
		}
	}
}

void Machine::UpdateInActive(void)
{
	SetInteractTime(COFFEE_PRODUCES_TIME);
}

void Machine::UpdateActive(void)
{
	param_.interactTime_ -= SceneManager::GetInstance().GetDeltaTime();

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
	int line = 3;	//�s
	int lineHeight = 30;	//�s

	VECTOR screenPos = ConvWorldPosToScreenPos(GetTransform().pos);
	// �ϊ�����
	DrawFormatString(static_cast<int>(screenPos.x) - 30, static_cast<int>(screenPos.y) - 150, GetColor(255, 255, 255),
		L"�R�[�q�[���ł���܂� %2.f", param_.interactTime_);

	StageObject::Draw();
}
