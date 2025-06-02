#include "../Manager/Generic/InputManager.h"
#include "../Object/Stage/StageManager.h"
#include "Counter.h"

Counter::Counter(const std::string objId, const float width,
	const float height, const float depth, Player& player,
	std::vector<std::unique_ptr<StageObject>>& objects) :
	StageObject(objId, width, height, depth, player), objects_(objects)
{
}

void Counter::Interact(const std::string& objId)
{
	auto& items = param_.acceptedItems_;
	//objId���C���^���N�g�Ώە��ɑ��݂��邩�ǂ���
	bool isAccepted = std::find(items.begin(), items.end(), objId) != items.end();
	//if (!isAccepted)return;	//���݂��Ȃ������珈�����Ȃ�


	//auto& ins = InputManager::GetInstance();
	//for (const auto& obj : objects_)
	//{
	//	//�R�[�q�[�ȊO�̃I�u�W�F�N�g�͔��肵�Ȃ�
	//	if (obj->GetObjectId() != HOT_COFFEE &&
	//		obj->GetObjectId() != ICE_COFFEE) continue;

	//	if (AsoUtility::IsHitSpheres(GetSpherePos(), GetSphereRad(),
	//		obj->GetSpherePos(), obj->GetSphereRad()))
	//	{
	//		//�X�y�[�X�L�[��������������ƃQ�[�W�����܂��Ă���
	//		if (isHoldingHotCoffee &&
	//			ins.IsNew(KEY_INPUT_SPACE))
	//		{
	//			param_.interactTime -= SceneManager::GetInstance().GetDeltaTime();
	//			isActioned_ = true;
	//		}
	//		else
	//		{
	//			param_.interactTime = 3.0f;
	//			isActioned_ = false;
	//		}
	//	}
	//}
}
