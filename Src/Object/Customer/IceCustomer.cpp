#include "../../Manager/Generic/SceneManager.h"
#include "../../Manager/Generic/ResourceManager.h"
#include "IceCustomer.h"

IceCustomer::IceCustomer(void)
{
}

void IceCustomer::Init(void)
{
}

void IceCustomer::Update(void)
{
}

void IceCustomer::Draw(void)
{
}

void IceCustomer::SetParam(void)
{
	//���f���̊�{�ݒ�
	transform_.SetModel(ResourceManager::GetInstance().LoadModelDuplicate(
		ResourceManager::SRC::PLAYER));

	//type_ = TYPE::ICE;

	SetType(TYPE::ICE);
}
