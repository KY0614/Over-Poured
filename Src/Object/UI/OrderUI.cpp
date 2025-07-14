#include "../../Manager/Generic/ResourceManager.h"
#include "OrderUI.h"

OrderUI::OrderUI(Order::DRINK drink, Order::SWEETS sweets)
{
	orderUIData_.drinkType_ = drink;
	orderUIData_.sweetsType_ = sweets;
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
}

void OrderUI::Update(void)
{
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
}
