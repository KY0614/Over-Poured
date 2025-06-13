#include "../Common/DebugDrawFormat.h"
#include "../Utility/AsoUtility.h"
#include "../Manager/Generic/SceneManager.h"
#include "../Manager/Generic/InputManager.h"
#include "../Object/Common/Sphere.h"
#include "../Object/Player.h"
#include "InteractObject.h"

InteractObject::InteractObject(const std::string objId,
	const float width, const float height,
	const float depth, Player& player,
	std::vector<std::unique_ptr<StageObject>>& object) :
	StageObject(objId, width, height, depth, player), objects_(object)
{
}

void InteractObject::Draw(void)
{
	int line = 3;	//�s
	int lineHeight = 30;	//�s

	VECTOR screenPos = ConvWorldPosToScreenPos(GetTransform().pos);
	// �ϊ�����
	DrawFormatString(static_cast<int>(screenPos.x) - 30, static_cast<int>(screenPos.y) - 150, GetColor(255, 255, 255),
		L"interactTime %2.f", param_.interactTime);

	StageObject::Draw();
}

void InteractObject::Interact(const std::string& objId)
{
	auto& items = param_.acceptedItems_;
	//objId���C���^���N�g�Ώە��ɑ��݂��邩�ǂ���
	bool isAccepted = std::find(items.begin(), items.end(), objId) != items.end();
	if (!isAccepted)return;	//���݂��Ȃ������珈�����Ȃ�

}

void InteractObject::UpdateInActive(void)
{
}

void InteractObject::UpdateActive(void)
{
}

void InteractObject::IceDispenser(void)
{
	if (GetMachineState() == MACHINE_STATE::ACTIVE)return;	//�A�N�e�B�u��Ԃ������珈�����Ȃ�

	
	for (const auto& obj : objects_)
	{
		//���Ƀ}�V�����ɃJ�b�v�܂��͕X����J�b�v������ꍇ�͉������Ȃ�
		if ((obj->GetObjectId() == "Ice_Cup" || obj->GetObjectId() == "Cup_With_Ice") &&
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
		if (obj->GetObjectId() != "Ice_Cup") continue;

		if (AsoUtility::IsHitSpheres(GetSpherePos(), GetSphereRad(),
			obj->GetSpherePos(), obj->GetSphereRad()))
		{
			//�X�y�[�X�L�[�����Ń}�V���̏ꏊ�ɃJ�b�v��u��(�Ƃ肠����)
			if (/*player_.GetHoldItem() == items.front().c_str() &&
				*/ins.IsTrgDown(KEY_INPUT_SPACE))
			{
				VECTOR cupPos = GetTopCenter();	//�}�V���̏㕔�����ɃJ�b�v��u��
				cupPos.y += obj->GetObjHeight() / 2.0f;	//������ɂ��炷
				obj->ItemPlaced(GetPos());
				ChangeMachineState(MACHINE_STATE::ACTIVE);
			}
		}
	}
}

void InteractObject::CoffeeMachine(void)
{
	//���Ƀ}�V�����ɃJ�b�v�܂��̓R�[�q�[������ꍇ�͉������Ȃ�
	for (const auto& obj : objects_)
	{
		// �z�b�g�J�b�v�܂��̓z�b�g�R�[�q�[
		if ((obj->GetObjectId() == "Hot_Cup" ||
			obj->GetObjectId() == "Hot_Coffee" ||
			obj->GetObjectId() == "Cup_With_Ice" ||
			obj->GetObjectId() == "Ice_Coffee") &&
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
		if (obj->GetObjectId() != "Hot_Cup" && obj->GetObjectId() != "Cup_With_Ice") continue;

		if (AsoUtility::IsHitSpheres(GetSpherePos(), GetSphereRad(),
			obj->GetSpherePos(), obj->GetSphereRad()))
		{
			//�X�y�[�X�L�[�����Ń}�V���̏ꏊ�ɃJ�b�v��u��
			if (/*(player_.GetHoldItem() == items.front().c_str() ||
				player_.GetHoldItem() == items.back().c_str()) &&*/
				ins.IsInputTriggered("Interact"))
			{
				VECTOR cupPos = GetTopCenter();	//�}�V���̏㕔�����ɃJ�b�v��u��
				cupPos.y += obj->GetObjHeight() / 2.0f;	//������ɂ��炷
				obj->ItemPlaced(cupPos);
				ChangeMachineState(MACHINE_STATE::ACTIVE);
			}
		}
	}
}

void InteractObject::CupLidRack(void)
{
	auto& ins = InputManager::GetInstance();
	for (const auto& obj : objects_)
	{
		//�R�[�q�[�ȊO�̃I�u�W�F�N�g�͔��肵�Ȃ�
		if (obj->GetObjectId() != "Hot_Coffee" &&
			obj->GetObjectId() != "Ice_Coffee") continue;
		//���ɊW����Ă���R�[�q�[�͔��肵�Ȃ�
		if (obj->IsLidOn())continue;

		bool isHoldingHotCoffee = obj->GetItemState() == ITEM_STATE::HOLD;

		if (AsoUtility::IsHitSpheres(GetSpherePos(), GetSphereRad(),
			obj->GetSpherePos(), obj->GetSphereRad()))
		{
			//�X�y�[�X�L�[��������������ƃQ�[�W�����܂��Ă���
			if (isHoldingHotCoffee &&
				ins.IsInputPressed("Interact"))
			{
				param_.interactTime -= SceneManager::GetInstance().GetDeltaTime();
				isActioned_ = true;
				player_.ChangeState(Player::STATE::STOP);
			}
			else
			{
				param_.interactTime = 3.0f;
				isActioned_ = false;
				player_.ChangeState(Player::STATE::PLAY);
			}
		}
	}
}
