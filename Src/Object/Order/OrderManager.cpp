#include <DxLib.h>
#include "Order.h"
#include "OrderManager.h"

namespace 
{
	//注文関連
	const int MAX_CREATE_NUM = 6;	//最大注文生成数
}

OrderManager::OrderManager(void)
{
	orders_.clear();
}

OrderManager::~OrderManager(void)
{
	orders_.clear();
}

void OrderManager::Init(void)
{
	//配列の初期化
	orders_.clear();

	//注文を最大数まで生成する
	CreateOrderMaxNum();
}

void OrderManager::FirstOrderUpdate(void)
{
	//ordersの中身が空だったら更新しない
	if (orders_.empty()) return;

	//先頭の注文の制限時間を減らす
	orders_.front()->TimerUpdate();
}

void OrderManager::AddCreateOrder(void)
{
	//最大注文生成数を超えそうだったら処理しない
	if (orders_.size() >= MAX_CREATE_NUM) return;

	//注文が最大数未満になったら１つ追加
	if (orders_.size() < MAX_CREATE_NUM)
	{
		CreateSingleOrder();
	}
}

void OrderManager::ClearFirstOrder(void)
{
	//先頭の要素を削除
	orders_.erase(orders_.begin());
}

const bool& OrderManager::IsFirstOrderTimeOut(void)
{
	//制限時間が切れたらtrueを返す
	if (orders_.front()->GetOrderTime() < 0.1f)	//0.0f以下だと誤差でマイナスになる可能性があるため0.1f以下にしている
	{
		return true;
	}
	return false;
}

const std::vector<Order::OrderData>& OrderManager::GetAllOrder(void) const
{
	//現在の全ての注文データを返す
	std::vector<Order::OrderData> retData;
	for (const auto& order : orders_)
	{
		retData.push_back(order->GetOrder());
	}
	return retData;
}

const Order::OrderData& OrderManager::GetLastOrderData(void) const
{
	//最後の注文データを返す
	return orders_.back()->GetOrder();
}

void OrderManager::CreateOrderMaxNum(void)
{
	//注文がすでに生成されていたら生成しない
	if (orders_.empty())
	{
		//最大数まで生成する
		for (int i = 0; i < MAX_CREATE_NUM; i++)
		{
			//１つ生成
			CreateSingleOrder();
		}
	}
}

void OrderManager::CreateSingleOrder(void)
{
	//最大注文生成数を超えそうだったら生成しない
	if (orders_.size() >= MAX_CREATE_NUM) return;

	//注文を生成する
	std::unique_ptr<Order> order = std::make_unique<Order>();
	order->CreateOrder();		//注文を生成
	orders_.push_back(std::move(order));
}