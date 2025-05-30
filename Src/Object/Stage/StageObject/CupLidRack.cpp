#include "../Common/DebugDrawFormat.h"
#include "../Utility/AsoUtility.h"
#include "../Manager/Generic/SceneManager.h"
#include "../Manager/Generic/InputManager.h"
#include "../Object/Player.h"
#include "HotCoffee.h"
#include "CupLidRack.h"

namespace {
	const std::string HOT_COFFEE = "Hot_Coffee";	//�z�b�g�R�[�q�[
	const std::string ICE_COFFEE = "Ice_Coffee";	//�A�C�X�R�[�q�[
}

CupLidRack::CupLidRack(const std::string objId, const float width,
	const float height, const float depth, Player& player,
	std::vector<std::unique_ptr<StageObject>>& object) :
	StageObject(objId, width, height, depth, player), objects_(object)
{
}

void CupLidRack::Interact(const std::string& objId)
{
	auto& items = param_.acceptedItems_;
	//objId���C���^���N�g�Ώە��ɑ��݂��邩�ǂ���
	bool isAccepted = std::find(items.begin(), items.end(), objId) != items.end();
	if (!isAccepted)return;	//���݂��Ȃ������珈�����Ȃ�

	auto& ins = InputManager::GetInstance();
	for (const auto& obj : objects_)
	{
		//�R�[�q�[�ȊO�̃I�u�W�F�N�g�͔��肵�Ȃ�
		if (obj->GetObjectId() != HOT_COFFEE && obj->GetObjectId() != ICE_COFFEE) continue;

		if (AsoUtility::IsHitSpheres(GetSpherePos(), GetSphereRad(),
			obj->GetSpherePos(), obj->GetSphereRad()))
		{
			//�X�y�[�X�L�[��������������ƃQ�[�W�����܂��Ă���
			if (player_.GetHoldItem() == HOT_COFFEE &&
				ins.IsNew(KEY_INPUT_SPACE))
			{
				isActioned_ = true;
				break;
			}
			else
			{
				isActioned_ = false;
				
			}
		}
		else
		{
			isActioned_ = false;
		}
	}

}

void CupLidRack::Update(void)
{
	if (isActioned_)
	{
		param_.interactTime -= SceneManager::GetInstance().GetDeltaTime();
	}
	else
	{
		param_.interactTime = 3.0f;
	}
}

void CupLidRack::Draw(void)
{
	int line = 4;	//�s
	int lineHeight = 30;	//�s
	DebugDrawFormat::FormatStringRight(L"LidTime %2.f", param_.interactTime, line, lineHeight);
	StageObject::Draw();
}
