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
	const std::string ICE_CUP = "Ice_Cup";	//�A�C�X�p�J�b�v
	const std::string CUP_WITH_ICE = "Cup_With_Ice";		//�A�C�X�p�J�b�v
}

Machine::Machine(const std::string objId, const float width,
	const float height, const float depth,Player& player,
	std::vector<std::unique_ptr<StageObject>>& object) :
	StageObject(objId, width, height, depth,player),objects_(object)
{
}

void Machine::Interact(const std::string& objId)
{
	auto& items = param_.acceptedItems_;
	//objId���C���^���N�g�Ώە��ɑ��݂��邩�ǂ���
	bool isAccepted = std::find(items.begin(), items.end(), objId) != items.end();
	if (!isAccepted)return;	//���݂��Ȃ������珈�����Ȃ�

	auto& ins = InputManager::GetInstance();
	for (const auto& obj : objects_)
	{
		//�z�b�g�p�J�b�v�ȊO�̃I�u�W�F�N�g�͔��肵�Ȃ�
		if (obj->GetObjectId() != HOT_CUP) continue;

		if (AsoUtility::IsHitSpheres(GetSpherePos(), GetSphereRad(),
			obj->GetSpherePos(), obj->GetSphereRad()))
		{
			//�X�y�[�X�L�[�����Ń}�V���̏ꏊ�ɃJ�b�v��u��(�Ƃ肠����)
			if (player_.GetHoldItem() == items.front().c_str() &&
				ins.IsTrgDown(KEY_INPUT_SPACE))
			{
				obj->ItemPlaced(StageManager::MACHINE_POS);
				ChangeMachineState(MACHINE_STATE::ACTIVE);
			}
		}
	}
}

void Machine::UpdateInActive(void)
{
	SetProduceTime(COFFEE_PRODUCES_TIME);
}

void Machine::UpdateActive(void)
{
	param_.interactTime -= SceneManager::GetInstance().GetDeltaTime();

	//�C���^���N�g���Ԃ��߂���������̓A�C�e�����ݒu��Ԃł͂Ȃ��������A�N�e�B�u�ɂ���
	for (const auto& obj : objects_)
	{
		if (param_.interactTime <= 0.0f ||
			(AsoUtility::IsHitSpheres(obj->GetSpherePos(), obj->GetSphereRad(),
			GetSpherePos(), GetSphereRad()) && obj->GetItemState() != ITEM_STATE::PLACED))
		{
			ChangeMachineState(MACHINE_STATE::INACTIVE);
		}
	}
}

void Machine::Init(void)
{
	StageObject::Init();
}

void Machine::Update(void)
{
	StageObject::Update();
}

void Machine::Draw(void)
{
	int line = 3;	//�s
	int lineHeight = 30;	//�s

	VECTOR screenPos = ConvWorldPosToScreenPos(GetTransform().pos);
	// �ϊ�����
	DrawFormatString(static_cast<int>(screenPos.x) - 30, static_cast<int>(screenPos.y) - 150, GetColor(255, 255, 255),
		L"�R�[�q�[���ł���܂� %2.f", param_.interactTime);

	//DebugDrawFormat::FormatStringRight(L"iteractTime %2.f", param_.interactTime, line, lineHeight);
	StageObject::Draw();
}