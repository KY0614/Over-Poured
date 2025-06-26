#include "../../Player.h"
#include "FollowingObject.h"
#include "DustBox.h"

namespace {
	const std::string HOT_COFFEE = "Hot_Coffee";		//�z�b�g�R�[�q�[
	const std::string ICE_COFFEE = "Ice_Coffee";		//�A�C�X�R�[�q�[
}

DustBox::DustBox(const std::string objId, const float width,
	const float height, const float depth, Player& player,
	std::vector<std::unique_ptr<StageObject>>& objects) :
	StageObject(objId, width, height, depth, player), objects_(objects)
{
}

void DustBox::Interact(const std::string& objId)
{
	auto& items = param_.acceptedItems_;
	//objId���C���^���N�g�Ώە��ɑ��݂��邩�ǂ���
	bool isAccepted = std::find(items.begin(), items.end(), objId) != items.end();
	if (!isAccepted)return;	//���݂��Ȃ������珈�����Ȃ�

	// �v���C���[�������Ă���A�C�e�������擾
	std::string heldItem = player_.GetHoldItem();

	// �R�[�q�[�{�̂̃C���f�b�N�X��T��
	int coffeeIndex = -1;
	for (int i = 0; i < objects_.size(); ++i)
	{
		if ((objects_[i]->GetParam().id_ == HOT_COFFEE || objects_[i]->GetParam().id_ == ICE_COFFEE) &&
			objects_[i]->GetItemState() == StageObject::ITEM_STATE::HOLD)
		{
			coffeeIndex = i;
			break;
		}
	}

	// �W�t���R�[�q�[�̏ꍇ�͊W���폜
	if (coffeeIndex != -1 && objects_[coffeeIndex]->IsLidOn())
	{
		// �W�̃C���f�b�N�X��T��
		for (int i = 0; i < objects_.size(); ++i)
		{
			// dynamic_cast��CupLid�^�ɕϊ����A�e�Q�Ƃ��r
			//�W���폜����
			FollowingObject* lid = dynamic_cast<FollowingObject*>(objects_[i].get());
			if (lid && &(lid->GetFollowedObj()) == objects_[coffeeIndex].get())
			{
				objects_.erase(objects_.begin() + i);
				continue; // �W���폜������A���̃��[�v��
			}
			++i;
		}
		// �R�[�q�[�{�̂��폜
		objects_.erase(objects_.begin() + coffeeIndex);
	}
	else
	{
		// �ʏ�̃I�u�W�F�N�g�폜
		for (auto it = objects_.begin(); it != objects_.end(); ++it)
		{
			if ((*it)->GetParam().id_ == heldItem &&
				(*it)->GetItemState() == StageObject::ITEM_STATE::HOLD)
			{
				objects_.erase(it);
				break;
			}
		}
	}

	// �v���C���[�̎�������Ԃ����Z�b�g
	player_.SetHoldItem("");
	player_.SetIsHoldiong(false);
}
