#include "../Common/DebugDrawFormat.h"
#include "../Utility/AsoUtility.h"
#include "../Manager/Generic/SceneManager.h"
#include "../Manager/Generic/InputManager.h"
#include "../Object/Common/Sphere.h"
#include "../Object/Player.h"
#include "../Object/Stage/StageManager.h"
#include "IceDispenser.h"

namespace {
	const std::string ICE_CUP = "Ice_Cup";	//�A�C�X�p�J�b�v
	const std::string CUP_WITH_ICE = "Cup_With_Ice";		//�A�C�X�p�J�b�v
}

IceDispenser::IceDispenser(const std::string objId, const float width,
	const float height, const float depth, Player& player,
	std::vector<std::unique_ptr<StageObject>>& object) :
	StageObject(objId, width, height, depth, player), objects_(object)
{
}

void IceDispenser::Draw(void)
{
	int line = 3;	//�s
	int lineHeight = 30;	//�s

	VECTOR screenPos = ConvWorldPosToScreenPos(GetTransform().pos);
	// �ϊ�����
	DrawFormatString(static_cast<int>(screenPos.x) - 30, static_cast<int>(screenPos.y) - 150, GetColor(255, 255, 255),
		L"�X�������܂� %2.f", param_.interactTime);

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
		if ((obj->GetObjectId() == ICE_CUP || obj->GetObjectId() == CUP_WITH_ICE) &&
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
		if (obj->GetObjectId() != ICE_CUP) continue;

		if (AsoUtility::IsHitSpheres(GetSpherePos(), GetSphereRad(),
			obj->GetSpherePos(), obj->GetSphereRad()))
		{
			//�X�y�[�X�L�[�����Ń}�V���̏ꏊ�ɃJ�b�v��u��(�Ƃ肠����)
			if (player_.GetHoldItem() == items.front().c_str() &&
				ins.IsTrgDown(KEY_INPUT_SPACE))
			{
				VECTOR cupPos = GetTopCenter();	//�}�V���̏㕔�����ɃJ�b�v��u��
				cupPos.y += obj->GetObjHeight() / 2.0f;	//������ɂ��炷
				obj->ItemPlaced(GetPos());
				ChangeMachineState(MACHINE_STATE::ACTIVE);
			}
		}
	}
}

void IceDispenser::UpdateInActive(void)
{
	SetProduceTime(COFFEE_PRODUCES_TIME);
}

void IceDispenser::UpdateActive(void)
{
	param_.interactTime -= SceneManager::GetInstance().GetDeltaTime();

	//�}�V���̓����蔻�����PLACED��Ԃ̃J�b�v�����݂��邩�`�F�b�N
	bool hasPlacedCup = false;
	for (const auto& obj : objects_)
	{
		if (obj->GetObjectId() != ICE_CUP)continue;

		if (AsoUtility::IsHitSpheres(obj->GetSpherePos(), obj->GetSphereRad(),
			GetSpherePos(), GetSphereRad()) &&
			obj->GetItemState() == ITEM_STATE::PLACED)
		{
			hasPlacedCup = true;
			break;
		}
		//else if (AsoUtility::IsHitSpheres(obj->GetSpherePos(), obj->GetSphereRad(),
		//	GetSpherePos(), GetSphereRad()) &&
		//	obj->GetItemState() != ITEM_STATE::PLACED)
		//{
		//	break;	//PLACED��Ԃ̃J�b�v���Ȃ���΃��[�v�𔲂���
		//}
	}

	//PLACED��Ԃ̃J�b�v���Ȃ���Δ�A�N�e�B�u�ɂ���
	if (!hasPlacedCup)
	{
		ChangeMachineState(MACHINE_STATE::INACTIVE);
		return;
	}

	//�C���^���N�g���Ԃ��߂������A�N�e�B�u�ɂ���
	if (param_.interactTime <= 0.0f)
	{
		ChangeMachineState(MACHINE_STATE::INACTIVE);
		return;
	}
}
