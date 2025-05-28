#include "../Manager/Generic/SceneManager.h"
#include "../Manager/Generic/InputManager.h"
#include "../Object/Player.h"
#include "../Object/Stage/StageManager.h"
#include "Machine.h"

namespace {
	const std::string NO_ITEM = "";	//�z�b�g�p�J�b�v
	const std::string HOT_CUP = "Hot_Cup";	//�z�b�g�p�J�b�v
	const std::string ICE_CUP = "Ice_Cup";	//�A�C�X�p�J�b�v
	const std::string CUP_WITH_ICE = "Cup_With_Ice";		//�A�C�X�p�J�b�v
	const std::string COFFEE_MACHINE = "Coffee_Machine";	//�R�[�q�[�}�V��
}

Machine::Machine(const std::string objId, const float width,
	const float height, const float depth,Player& player) : 
	StageObject(objId, width, height, depth,player)
{
	param_.interactTime = 6.0f;
}

void Machine::Interact(const std::string& objId, std::vector<std::unique_ptr<StageObject>>& object)
{
	auto& items = param_.acceptedItems_;
	//objId���C���^���N�g�Ώە��ɑ��݂��邩�ǂ���
	bool isAccepted = std::find(items.begin(), items.end(), objId) != items.end();
	if (!isAccepted)return;	//���݂��Ȃ������珈�����Ȃ�

	auto& ins = InputManager::GetInstance();
	bool setCup = false;
	for (const auto& obj : object)
	{
		if (player_.GetIsHolding())
		{
			//�X�y�[�X�L�[�����Ń}�V���̏ꏊ�ɃJ�b�v��u��(�Ƃ肠����)
			if (player_.GetHoldItem() == items.front().c_str() &&
				ins.IsTrgDown(KEY_INPUT_SPACE))
			{
				obj->ItemPlaced(StageManager::MACHINE_POS);
				setCup = true;
			}
		}
		else if (setCup)
		{
			//�u������R�[�q�[�𒍂�
			if (player_.GetHoldItem() == NO_ITEM)
			{
				ChangeMachineState(MACHINE_STATE::ACTIVE);
			}
		}
	}

	//�X�y�[�X�L�[�����Ń}�V���̏ꏊ�ɃJ�b�v��u��(�Ƃ肠����)
	//if (ins.IsTrgDown(KEY_INPUT_SPACE) &&
	//	player_.GetHoldItem() == CUP_WITH_ICE)
	//{
	//	player_.SetIsHoldiong(false);
	//	cupI_.ChangeItemState(StageObject::ITEM_STATE::PLACED);
	//	cupI_.SetPos(StageManager::MACHINE_POS);
	//}
}

void Machine::UpdateInActive(void)
{
	param_.interactTime = 6.0f;
}

void Machine::UpdateActive(void)
{
	param_.interactTime -= SceneManager::GetInstance().GetDeltaTime();
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
	StageObject::Draw();
}