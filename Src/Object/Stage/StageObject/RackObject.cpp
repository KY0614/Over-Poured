#include "../Manager/Generic/InputManager.h"
#include "../../Common/Sphere.h"
#include "../Object/Player.h"
#include "ItemObject.h"
#include "RackObject.h"

RackObject::RackObject(const std::string objId,
	const float width, const float height,
	const float depth, Player& player) :
	StageObject(objId, width, height, depth, player)
{
}

void RackObject::PickUp(std::string rackName,std::vector<std::unique_ptr<StageObject>>& object)
{
	auto& ins = InputManager::GetInstance();
	//�z�b�g�J�b�v�p���b�N�ɃC���^���N�g�����Ƃ��̏���
	if (rackName == "Cup_Hot_Rack" && ins.IsInputTriggered("Interact"))
	{
		object.emplace_back(std::make_unique<ItemObject>("Hot_Cup", 40.0f, 30.0f, 40.0f, player_));
		//object.back()->SetPos(player_.GetSphere().GetPos());
		object.back()->Init(player_.GetSphere().GetPos());
		player_.SetHoldItem(object.back()->GetParam().id_);
		object.back()->ChangeItemState(ITEM_STATE::HOLD);
		isActioned_ = true;
	}
	//�A�C�X�J�b�v�p���b�N�ɃC���^���N�g�����Ƃ��̏���
	if (rackName == "Cup_Ice_Rack" && ins.IsInputTriggered("Interact"))
	{
		object.emplace_back(std::make_unique<ItemObject>("Ice_Cup", 40.0f, 30.0f, 40.0f, player_));
		//object.back()->SetPos(player_.GetSphere().GetPos());
		object.back()->Init(player_.GetSphere().GetPos());
		player_.SetHoldItem(object.back()->GetParam().id_);
		object.back()->ChangeItemState(ITEM_STATE::HOLD);
		isActioned_ = true;
	}
	//�X�C�[�c�i�x���[�j�p���b�N�ɃC���^���N�g�����Ƃ��̏���
	if (rackName == "Sweets_Strawberry_Rack" && ins.IsInputTriggered("Interact"))
	{
		//�X�C�[�c�����o��
		object.emplace_back(std::make_unique<ItemObject>("Sweets_Strawberry", 40.0f, 30.0f, 40.0f, player_));
		//object.back()->SetPos(player_.GetSphere().GetPos());
		object.back()->Init(player_.GetSphere().GetPos());
		player_.SetHoldItem(object.back()->GetParam().id_);
		object.back()->ChangeItemState(ITEM_STATE::HOLD);
		isActioned_ = true;
	}
	//�X�C�[�c�i�`���R�j�p���b�N�ɃC���^���N�g�����Ƃ��̏���
	if (rackName == "Sweets_Choco_Rack" && ins.IsInputTriggered("Interact"))
	{
		//�X�C�[�c�����o��
		object.emplace_back(std::make_unique<ItemObject>("Sweets_Choco", 40.0f, 30.0f, 40.0f, player_));
		//object.back()->SetPos(player_.GetSphere().GetPos());
		object.back()->Init(player_.GetSphere().GetPos());
		player_.SetHoldItem(object.back()->GetParam().id_);
		object.back()->ChangeItemState(ITEM_STATE::HOLD);
		isActioned_ = true;
	}
}
