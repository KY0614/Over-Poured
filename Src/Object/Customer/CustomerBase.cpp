#include<DxLib.h>
#include "../../Utility/AsoUtility.h"
#include "CustomerBase.h"

CustomerBase::CustomerBase(void)
{
	type_ = TYPE::NONE;
}

void CustomerBase::Update(void)
{
	transform_.Update();

	Move();
}

void CustomerBase::Move(void)
{
}
