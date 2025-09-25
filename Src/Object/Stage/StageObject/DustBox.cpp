#include "../../Player.h"
#include "FollowingObject.h"
#include "ItemObject.h"
#include "DustBox.h"

DustBox::DustBox(const std::string objId,Player& player,
	std::vector<std::unique_ptr<StageObject>>& objects) :
	StageObject(objId, player), objects_(objects)
{
}

void DustBox::Interact(const std::string& objId)
{
	auto& items = param_.acceptedItems_;
	//objId���C���^���N�g�Ώە��ɑ��݂��邩�ǂ���
	bool isAccepted = std::find(items.begin(), items.end(), objId) != items.end();
	if (!isAccepted)return;	//���݂��Ȃ������珈�����Ȃ�

	//�v���C���[�������Ă���A�C�e�������擾
	std::string heldItem = player_.GetHoldItem();

	//�R�[�q�[�{�̂̃C���f�b�N�X��T��
	int coffeeIndex = -1;
	for (int i = 0; i < objects_.size(); ++i)
	{
		//�����Ă���̂��R�[�q�[�n�̏ꍇ�C���f�b�N�X��ۑ�
		if ((objects_[i]->GetParam().id_ == HOT_COFFEE ||
			objects_[i]->GetParam().id_ == CUP_WITH_ICE||
			objects_[i]->GetParam().id_ == ICE_COFFEE) &&
			objects_[i]->GetItemState() == StageObject::ITEM_STATE::HOLD)
		{
			coffeeIndex = i;
			break;
		}
	}
	//�X����J�b�v�̏ꍇ
	if (coffeeIndex != -1)
	{
		//�X����J�b�v�̏ꍇ�͕X���폜
		ItemObject* cupWithIce = dynamic_cast<ItemObject*>(objects_[coffeeIndex].get());
		if (cupWithIce->IsIce())
		{
			//�W�̃C���f�b�N�X��T��
			for (int i = 0; i < objects_.size(); ++i)
			{
				//dynamic_cast��FollowingObject�^�ɕϊ����A�e�Q�Ƃ��r
				//�W���폜����
				FollowingObject* follower = dynamic_cast<FollowingObject*>(objects_[i].get());
				if (follower && &(follower->GetFollowedObj()) == objects_[coffeeIndex].get())
				{
					objects_.erase(objects_.begin() + i);
					continue; //erase�����̂�i�͐i�߂����̃��[�v��
				}
			}
		}
	}

	//�W�t���R�[�q�[�̏ꍇ�͊W���폜
	if (coffeeIndex != -1 && objects_[coffeeIndex]->IsLidOn())
	{
		//�W�̃C���f�b�N�X��T��
		for (int i = 0; i < objects_.size(); ++i)
		{
			//dynamic_cast��FollowingObject�^�ɕϊ����A�e�Q�Ƃ��r
			//�W���폜����
   			FollowingObject* follower = dynamic_cast<FollowingObject*>(objects_[i].get());
			if (follower && &(follower->GetFollowedObj()) == objects_[coffeeIndex].get())
			{
				objects_.erase(objects_.begin() + i);
				continue; // erase�����̂�i�͐i�߂����̃��[�v��
			}
		}
		//�R�[�q�[�{�̂��폜
		objects_.erase(objects_.begin() + coffeeIndex);
	}
	else
	{
		//�ʏ�̃I�u�W�F�N�g�폜
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

	//�v���C���[�̎�������Ԃ����Z�b�g
	player_.SetHoldItem("");
	player_.SetIsHolding(false);
}
