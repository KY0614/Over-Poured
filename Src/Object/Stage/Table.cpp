#include "../Manager/Generic/InputManager.h"
#include "../Utility/AsoUtility.h"
#include "../Object/Common/Sphere.h"
#include "../Object/Player.h"
#include "StageManager.h"
#include "Table.h"

Table::Table(const std::string objId, const float width,
	const float height, const float depth, Player& player,
	std::vector<std::unique_ptr<StageObject>>& objects) :
	StageObject(objId, width, height, depth, player),objects_(objects)
{
	
}

void Table::Update(void)
{
	//�����蔻��p�̋��̂��e�[�u���̍����ƍ��킹��
	sphereTran_.pos.y = height_;

	for (const auto& obj : objects_)
	{
		if (AsoUtility::IsHitSpheres(obj->GetSpherePos(), obj->GetSphereRad(),
			sphereTran_.pos, rad_) && obj->GetItemState() == StageObject::ITEM_STATE::PLACED)
		{
			param_.placeable_ = false;
			break;
		}
		else if(obj->GetItemState() != StageObject::ITEM_STATE::PLACED)
		{
			param_.placeable_ = true;
		}
	}
}

void Table::Interact(void)
{
}