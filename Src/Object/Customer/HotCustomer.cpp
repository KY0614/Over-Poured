#include "../../Utility/AsoUtility.h"
#include "../../Manager/Generic/SceneManager.h"
#include "../../Manager/Generic/ResourceManager.h"
#include "HotCustomer.h"

HotCustomer::HotCustomer(void)
{
}

void HotCustomer::Init(void)
{
	SetParam();
}

void HotCustomer::Update(void)
{
	transform_.Update();
}

void HotCustomer::Draw(void)
{
	MV1DrawModel(transform_.modelId);
}

void HotCustomer::SetParam(void)
{
	//モデルの基本設定
	transform_.SetModel(ResourceManager::GetInstance().LoadModelDuplicate(
		ResourceManager::SRC::CUSTOMER));
	transform_.scl = AsoUtility::VECTOR_ONE;
	transform_.pos = { 0.0f, 0.0f, 271.0f };
	transform_.quaRot = Quaternion();
	transform_.quaRotLocal =
		Quaternion::Euler({ 0.0f, AsoUtility::Deg2RadF(180.0f), 0.0f });
	transform_.Update();

	//type_ = TYPE::HOT;

	SetType(TYPE::HOT);

	//テストでモデルの色を赤にする
	MV1SetMaterialDifColor(transform_.modelId, 0, GetColorF(1.0f, 0.0f, 0.0f, 1.0f));
}
