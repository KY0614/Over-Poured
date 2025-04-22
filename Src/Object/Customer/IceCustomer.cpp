#include "../../Utility/AsoUtility.h"
#include "../../Manager/Generic/SceneManager.h"
#include "../../Manager/Generic/ResourceManager.h"
#include "IceCustomer.h"

IceCustomer::IceCustomer(void)
{
}

void IceCustomer::Init(void)
{
	SetParam();
}

void IceCustomer::Update(void)
{
	transform_.Update();

}

void IceCustomer::Draw(void)
{
	MV1DrawModel(transform_.modelId);
}

void IceCustomer::SetParam(void)
{
	//モデルの基本設定
	transform_.SetModel(ResourceManager::GetInstance().LoadModelDuplicate(
		ResourceManager::SRC::CUSTOMER));
	transform_.scl = AsoUtility::VECTOR_ONE;
	transform_.pos = { 0.0f, 0.0f, 0.0f };
	transform_.quaRot = Quaternion();
	transform_.quaRotLocal =
		Quaternion::Euler({ 0.0f, AsoUtility::Deg2RadF(180.0f), 0.0f });
	transform_.Update();

	//type_ = TYPE::ICE;

	SetType(TYPE::ICE);

	//テストでモデルの色を青にする
	MV1SetMaterialDifColor(transform_.modelId, 0, GetColorF(0.0f, 0.0f, 1.0f, 1.0f));
}
