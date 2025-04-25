#include<DxLib.h>
#include "../../Utility/AsoUtility.h"
#include "CustomerBase.h"

CustomerBase::CustomerBase(void)
{
	type_ = TYPE::NONE;
	state_ = STATE::IDLE;
}

void CustomerBase::Init(void)
{
	transform_.scl = AsoUtility::VECTOR_ONE;
	transform_.pos = { 221.0f, 0.0f, 271.0f };
	transform_.quaRot = Quaternion();
	transform_.quaRotLocal =
		Quaternion::Euler({ 0.0f, AsoUtility::Deg2RadF(0.0f), 0.0f });

	transform_.Update();

	SetParam();
}

void CustomerBase::Update(void)
{
	transform_.Update();

	Move();
}

void CustomerBase::Move(void)
{
}
