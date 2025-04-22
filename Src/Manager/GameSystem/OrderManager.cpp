#include <DxLib.h>
#include "OrderManager.h"

OrderManager* OrderManager::instance_ = nullptr;

void OrderManager::CreateInstance(void)
{
	if (instance_ == nullptr)
	{
		instance_ = new OrderManager();
	}
	instance_->Init();
}

OrderManager& OrderManager::GetInstance(void)
{
	if (instance_ == nullptr)
	{
		OrderManager::CreateInstance();
	}
	return *instance_;
}

void OrderManager::Init(void)
{
	orderNum_ = 0;
}

void OrderManager::Update(void)
{
}

void OrderManager::Destroy(void)
{
	delete instance_;
}

OrderManager::Order OrderManager::CreateOrder(void)
{
	int drinkType = 0;
	int sweetsType = 0;

	//注文数を決める
	do
	{
		orderNum_ = GetRand(ORDER_MAX_NUM);
	} while (orderNum_ == 0);	//0だったら乱数取得しなおす

	//注文内容を決める
	switch (orderNum_)
	{
	case 1:
		do
		{
			drinkType = GetRand(DRINK_MAX_NUM);
		} while (drinkType == 0);	//0だったら乱数取得しなおす

		sweetsType = 0;		//注文数が１の時はスイーツは頼まないようにする
		SetDrink(static_cast<DRINK>(drinkType));	//飲み物の種類を設定
		SetSweets(static_cast<SWEETS>(sweetsType));	//食べ物の種類を設定

		SetOrderTime(ONE_ORDER_TIME);				//制限時間を設定
		break;

	case 2:

		do
		{
			drinkType = GetRand(DRINK_MAX_NUM);
		} while (drinkType == 0);	//0だったら乱数取得しなおす

		do
		{
			sweetsType = GetRand(FOOD_MAX_NUM);
		} while (sweetsType == 0);	//0だったら乱数取得しなおす

		SetDrink(static_cast<DRINK>(drinkType));	//飲み物の種類を設定
		SetSweets(static_cast<SWEETS>(sweetsType));	//食べ物の種類を設定

		SetOrderTime(TWO_ORDER_TIME);				//制限時間を設定
		break;

	default:
		break;
	}

	return order_;
}
