#include<DxLib.h>
#include "../../Utility/AsoUtility.h"
#include "CustomerBase.h"

CustomerBase::CustomerBase(void)
{
	type_ = TYPE::NONE;
}

void CustomerBase::Destroy(void)
{
}

void CustomerBase::Init(void)
{
	SetParam();

	transform_.scl = AsoUtility::VECTOR_ONE;
	transform_.pos = { 0.0f, 0.0f, 0.0f };
	transform_.quaRot = Quaternion();
	transform_.quaRotLocal =
		Quaternion::Euler({ 0.0f, AsoUtility::Deg2RadF(180.0f), 0.0f });
	transform_.Update();
}

void CustomerBase::Update(void)
{
	transform_.Update();
}

void CustomerBase::Draw(void)
{
	//ƒ‚ƒfƒ‹‚Ì•`‰æ
	MV1DrawModel(transform_.modelId);
}

void CustomerBase::Move(void)
{
}
