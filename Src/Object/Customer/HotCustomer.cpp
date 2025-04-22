#include "../../Manager/Generic/SceneManager.h"
#include "../../Manager/Generic/ResourceManager.h"
#include "HotCustomer.h"

HotCustomer::HotCustomer(void)
{
}

void HotCustomer::Init(void)
{
}

void HotCustomer::Update(void)
{
}

void HotCustomer::Draw(void)
{
}

void HotCustomer::SetParam(void)
{
	//ƒ‚ƒfƒ‹‚ÌŠî–{İ’è
	transform_.SetModel(ResourceManager::GetInstance().LoadModelDuplicate(
		ResourceManager::SRC::PLAYER));

	//type_ = TYPE::HOT;

	SetType(TYPE::HOT);
}
