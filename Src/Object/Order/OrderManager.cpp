#include <DxLib.h>
#include "../../Common/DebugDrawFormat.h"
#include "Order.h"
#include "OrderManager.h"

OrderManager::OrderManager(void)
{
}

OrderManager::~OrderManager(void)
{
	orders_.clear();
}

void OrderManager::Init(void)
{
	orders_.clear();
	CreateOrderMaxNum();
}

void OrderManager::FirstOrderUpdate(void)
{
	//orders�̒��g���󂾂�����X�V���Ȃ�
	if (orders_.empty()) return;

	//�擪�̒����̐������Ԃ����炷
	orders_.front()->TimerUpdate();
}

void OrderManager::CreateOrderMaxNum(void)
{
	if (orders_.empty())
	{
		//�ő吔��6�܂Ő�������
		for (int i = 0; i < MAX_CREATE_NUM; i++)
		{
			//�P����
			CreateSingleOrder();
		}
	}
}

void OrderManager::CreateSingleOrder(void)
{
	//�ő咍���������𒴂������������琶�����Ȃ�
	if (orders_.size() >= MAX_CREATE_NUM) return;
	//�����𐶐�����
	std::unique_ptr<Order> order = std::make_unique<Order>();
	order->CreateOrder();		//�����𐶐�
	orders_.push_back(std::move(order));
}

void OrderManager::AddOrder(void)
{
	//�ő咍���������𒴂�������������return
	if (orders_.size() >= MAX_CREATE_NUM) return;

	//�������S�ȉ��ɂȂ�����P�ǉ�
	if (orders_.size() <= MAX_CREATE_NUM - 1)
	{
		CreateSingleOrder();
	}
}

bool OrderManager::IsFirstOrderTimeOut(void)
{
	// �������Ԃ��؂ꂽ��true��Ԃ�
	if (orders_.front()->GetOrderTime() < 0.1f) 
	{
		return true;
	}
	return false;
}

void OrderManager::ClearFirstOrder(void)
{
	//�擪�̗v�f���폜
	orders_.erase(orders_.begin());
	//count_++;
}

std::vector<Order::DRINK> OrderManager::GetAllOrderDrink(void) const
{
	std::vector<Order::DRINK> drink;
	for (auto& order : orders_)
	{
		drink.push_back(order->GetOrder().drink_);
	}

	return drink;
}

std::vector<Order::SWEETS> OrderManager::GetAllOrderSweet(void) const
{
	std::vector<Order::SWEETS> sweets;
	for (auto& order : orders_)
	{
		sweets.push_back(order->GetOrder().sweets_);
	}

	return sweets;
}

std::vector<Order::OrderData> OrderManager::GetAllOrder(void) const
{
	std::vector<Order::OrderData> data;
	for (auto& order : orders_)
	{
		data.push_back(order->GetOrder());
	}

	return data;
}

Order::OrderData OrderManager::GetLastOrderData(void) const
{
	return orders_.back()->GetOrder();
}
