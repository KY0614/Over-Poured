#include "../../Application.h"
#include "../Common/Easing.h"
#include "../../Manager/Generic/ResourceManager.h"
#include "../../Manager/Generic/SceneManager.h"
#include "OrderUI.h"

OrderUI::OrderUI(Order::DRINK drink, Order::SWEETS sweets, float maxTime)
{
	orderUIData_.drinkType_ = drink;
	orderUIData_.sweetsType_ = sweets;
	orderMaxTime_ = maxTime;
	orderTimer_ = 0.0f;
	orderUIData_.currentRate_ = 0.0f;
	orderUIData_.displayedRate_ = 0.0f;
	gaugeTime_ = 0.0f;
	isActive_ = false;
	alpha_ = 1.0f;
	size_ = 100.0f;
}

void OrderUI::Init(void)
{
	orderUIData_.backUIImg_ = ResourceManager::GetInstance().Load(
		ResourceManager::SRC::UI_ORDER_BACK).handleId_;

	//ドリンク用UI
	if (orderUIData_.drinkType_ == Order::DRINK::HOT)
	{
		orderUIData_.drinkUIImg_ = ResourceManager::GetInstance().Load(
			ResourceManager::SRC::UI_HOT).handleId_;
	}
	else if (orderUIData_.drinkType_ == Order::DRINK::ICE)
	{
		orderUIData_.drinkUIImg_ = ResourceManager::GetInstance().Load(
			ResourceManager::SRC::UI_ICE).handleId_;
	}
	//スイーツ用UI
	if (orderUIData_.sweetsType_ == Order::SWEETS::CHOCO)
	{
		orderUIData_.sweetsUIImg_ = ResourceManager::GetInstance().Load(
			ResourceManager::SRC::UI_CHOCO).handleId_;
	}
	else if (orderUIData_.sweetsType_ == Order::SWEETS::STRAWBERRY)
	{
		orderUIData_.sweetsUIImg_ = ResourceManager::GetInstance().Load(
			ResourceManager::SRC::UI_BERRY).handleId_;
	}
	else
	{
		orderUIData_.sweetsUIImg_ = -1;
	}

	orderUIData_.timerBackImg_ = ResourceManager::GetInstance().Load(
		ResourceManager::SRC::UI_CIRCLESHADOW).handleId_;

	orderUIData_.timerImg_ = ResourceManager::GetInstance().Load(
		ResourceManager::SRC::UI_CIRCLE).handleId_;

}

void OrderUI::Update(void)
{
	if (!isActive_)return;
	orderUIData_.displayedRate_ = Easing::Linear(
		gaugeTime_, 1.0f,
		0.0f, orderUIData_.currentRate_);

	//スコアを現在のランクの範囲を比例計算する（後で100をかけてパーセントにする）
	orderUIData_.currentRate_ = orderTimer_ / orderMaxTime_;
	if (orderUIData_.currentRate_ <= 0.0f)orderUIData_.currentRate_ = 0.0f;
}

void OrderUI::Draw(void)
{
	if (isActive_ != true)return;

	DrawBillboard3D(pos_, 0.5f, 0.5f, BACK_IMG_SIZE,
		0.0f, orderUIData_.backUIImg_, true);

	if (orderUIData_.sweetsType_ != Order::SWEETS::NONE)
	{
		VECTOR drinkPos = VAdd(pos_, VGet(-(size_ / 2.0f), 0.0f, 0.0f));
		VECTOR sweetsPos = VAdd(pos_, VGet((size_ / 2.0f),0.0f,0.0f));
		DrawBillboard3D(drinkPos, 0.5f, 0.5f, size_,
			0.0f, orderUIData_.drinkUIImg_, true);

		DrawBillboard3D(sweetsPos, 0.5f, 0.5f, size_,
			0.0f, orderUIData_.sweetsUIImg_, true);
	}
	else
	{
		DrawBillboard3D(pos_, 0.5f, 0.5f, size_,
			0.0f, orderUIData_.drinkUIImg_, true);
	}

	VECTOR pos = VAdd(pos_, VGet(120.0f, -50.0f, 0.0f));
	VECTOR screenPos = ConvWorldPosToScreenPos(pos);
	DrawBillboard3D(pos,
		0.5f, 0.5f, size_,
		0.0f, orderUIData_.timerBackImg_, true);
	//画面の大きさに合わせて拡大率を変える
	float scale = 
		static_cast<float>(Application::SCREEN_SIZE_Y) / 
		static_cast<float>(Application::SCREEN_MAX_SIZE_Y);
	DrawCircleGauge(
		(int)screenPos.x,
		(int)screenPos.y,
		orderUIData_.currentRate_ * 100.0f,
		orderUIData_.timerImg_,
		0.0f,
		scale,
		false, false
	);
}
