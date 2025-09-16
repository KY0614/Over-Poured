#include <DxLib.h>
#include "../Libs/ImGui/imgui.h"
#include "../Common/DebugDrawFormat.h"
#include "../Manager/Generic/SceneManager.h"
#include "../Manager/Generic/InputManager.h"
#include "OrderManager.h"
#include "Order.h"

namespace 
{
	//注文関連
	const int ORDER_MAX_NUM = 2;			//注文の最大数
	const int ORDER_MIN_NUM = 1;			//注文の最小数

	const float ONE_ORDER_TIME = 25.0f;	//注文の制限時間(1つ)
	const float TWO_ORDER_TIME = 33.0f;	//注文の制限時間(２つ)

	//商品関連
	const int DRINK_MAX_NUM = 2;			//飲み物類の最大数
	const int FOOD_MAX_NUM = 2;			//食べ物類の最大数
}

Order::Order(void): orderData_()
{
}

Order::~Order(void)
{
}

void Order::TimerUpdate(void)
{
	//制限時間から経過時間を引いていく
	orderData_.time_ -= SceneManager::GetInstance().GetDeltaTime();
}

void Order::CreateOrder(void)
{
	int drinkType = 0;
	int sweetsType = 0;
	int orderNum = 0;

	//注文数を決める
	orderNum = GetRand(ORDER_MAX_NUM);
	while(orderNum <= 0)
	{
		//乱数が0以下だったらもう一度乱数を取得する
		orderNum = GetRand(ORDER_MAX_NUM);
	}

	//注文数を設定
	SetOrderNum(orderNum);	

	//飲み物の種類を乱数で取得
	while (drinkType <= 0)	//NONE(0)以下だったら乱数取得しなおす
	{
		drinkType = GetRand(DRINK_MAX_NUM);
	}

	//飲み物の種類を設定
	SetDrink(static_cast<DRINK>(drinkType));

	//注文数ごとの制限時間を設定
	if (orderNum > 0)
	{
		//制限時間を設定（注文数が1の時）
		SetOrderTime(ONE_ORDER_TIME);
	}
	else if(orderNum > ORDER_MIN_NUM)
	{
		//制限時間を設定（注文数が2の時）
		SetOrderTime(TWO_ORDER_TIME);
	}

	if (orderNum < ORDER_MAX_NUM)
	{
		//注文数が1の時はスイーツは頼まないようにする
		return;
	}

	//スイーツの種類を乱数で取得
	while (sweetsType <= 0)	//スイーツの種類がNONE(0)以下だったら乱数取得しなおす
	{
		sweetsType = GetRand(FOOD_MAX_NUM);
	}

	//食べ物の種類を設定
	SetSweets(static_cast<SWEETS>(sweetsType));
}