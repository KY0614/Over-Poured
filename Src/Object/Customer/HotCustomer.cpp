#include "../../Application.h"
#include "../../Utility/AsoUtility.h"
#include "../../Manager/Generic/SceneManager.h"
#include "../../Manager/Generic/ResourceManager.h"
#include "../Common/AnimationController.h"
#include "HotCustomer.h"

HotCustomer::HotCustomer(void)
{
	color_ = GetColorF(0.0f, 0.0f, 0.0f, 1.0f);
}

HotCustomer::~HotCustomer(void)
{
}

void HotCustomer::Draw(void)
{
	if (!isVisible_) return;
	UpdateCol();
	MV1DrawModel(transform_.modelId);
}

void HotCustomer::SetParam(void)
{
	//モデルの基本設定
	transform_.SetModel(ResourceManager::GetInstance().LoadModelDuplicate(
		ResourceManager::SRC::HOT_CUSTOMER));

	transform_.Update();

	SetType(TYPE::HOT);

	color_ = GetColorF(1.0f, 0.0f, 0.0f, 1.0f);

	//テストでモデルの色を赤にする
	//MV1SetMaterialDifColor(transform_.modelId, 0, color_);

	isVisible_ = true;
}

void HotCustomer::InitAnimation(void)
{
	std::string path = Application::PATH_MODEL + "Player/";
	animationController_ = std::make_unique<AnimationController>(transform_.modelId);
	animationController_->Add((int)STATE::IDLE, path + "Idle.mv1", 20.0f);
	animationController_->Add((int)STATE::WALK, path + "Walk.mv1", 30.0f);

	animationController_->Play((int)STATE::IDLE);
}

void HotCustomer::UpdateCol(void)
{
	switch (reaction_)
	{
	case CustomerBase::REACTION::GOOD:
		color_ = GetColorF(1.0f, 1.0f, 1.0f, 1.0f);
		MV1SetMaterialDifColor(transform_.modelId, 0, color_);
		break;

	case CustomerBase::REACTION::SOSO:
		color_ = GetColorF(0.4f, 0.4f, 0.4f, 1.0f);
		MV1SetMaterialDifColor(transform_.modelId, 0, color_);
		break;

	case CustomerBase::REACTION::BAD:
		color_ = GetColorF(0.0f, 0.0f, 0.0f, 1.0f);
		MV1SetMaterialDifColor(transform_.modelId, 0, color_);
		break;
	default:
		break;
	}
}