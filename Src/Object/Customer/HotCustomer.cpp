#include "../../Application.h"
#include "../../Utility/AsoUtility.h"
#include "../../Manager/Generic/SceneManager.h"
#include "../../Manager/Generic/ResourceManager.h"
#include "../Common/AnimationController.h"
#include "HotCustomer.h"

HotCustomer::HotCustomer(void)
{
}

HotCustomer::~HotCustomer(void)
{
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

	transform_.Update();

	SetType(TYPE::HOT);

	//テストでモデルの色を赤にする
	MV1SetMaterialDifColor(transform_.modelId, 0, GetColorF(1.0f, 0.0f, 0.0f, 1.0f));
}

void HotCustomer::InitAnimation(void)
{
	std::string path = Application::PATH_MODEL + "Customer/";
	animationController_ = std::make_unique<AnimationController>(transform_.modelId);
	animationController_->Add((int)STATE::IDLE, path + "Idle2.mv1", 20.0f);

	animationController_->Play((int)STATE::IDLE);
}
