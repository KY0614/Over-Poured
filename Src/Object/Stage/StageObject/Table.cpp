#include "../Utility/CommonUtility.h"
#include "../Object/Common/Sphere.h"
#include "../Object/Player.h"
#include "Table.h"

Table::Table(const std::string objId,Player& player,
	std::vector<std::unique_ptr<StageObject>>& objects) :
	StageObject(objId, player),objects_(objects)
{
}

void Table::Update(void)
{
	StageObject::Update();
	tableColliderTran_.Update();
	//�e�[�u���̏�ɃI�u�W�F�N�g���u����Ă��Ȃ����ǂ���
	for (const auto& obj : objects_)
	{
		if (CommonUtility::IsHitSpheres(obj->GetSpherePos(), obj->GetSphereRad(),
			GetSpherePos(), GetSphereRad()) && obj->GetItemState() == StageObject::ITEM_STATE::PLACED)
		{
			//�u����Ă���ꍇ�́A�u���Ȃ���Ԃɂ���
			param_.placeable_ = false;
			break;
		}
		else if(obj->GetItemState() != StageObject::ITEM_STATE::PLACED)
		{
			param_.placeable_ = true;
		}
	}
}