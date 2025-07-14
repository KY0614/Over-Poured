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
	InitOrder();
}

void OrderManager::FirstOrderUpdate(void)
{
	//orders‚Ì’†g‚ª‹ó‚¾‚Á‚½‚ç‚µ‚È‚¢
	if (orders_.empty()) return;

	//æ“ª‚Ì’•¶‚ğ•\¦‚µA§ŒÀŠÔ‚¾‚¯‚ğŒ¸‚ç‚·
	orders_.front()->Update();
}

void OrderManager::Draw(void)
{
}

void OrderManager::InitOrder(void)
{
	if (orders_.empty())
	{
		//6‚Â‚Ü‚Å¶¬‚·‚é
		for (int i = 0; i < MAX_CREATE_NUM; i++)
		{
			CreateSingleOrder();
		}
	}
}

void OrderManager::CreateSingleOrder(void)
{
	//Å‘å’•¶¶¬”‚ğ’´‚¦‚»‚¤‚¾‚Á‚½‚çreturn
	if (orders_.size() >= MAX_CREATE_NUM) return;

	std::unique_ptr<Order> order = std::make_unique<Order>();
	order->CreateOrder();		//’•¶‚ğ¶¬
	orders_.push_back(std::move(order));
}

void OrderManager::AddOrder(void)
{
	//Å‘å’•¶¶¬”‚ğ’´‚¦‚»‚¤‚¾‚Á‚½‚çreturn
	if (orders_.size() >= MAX_CREATE_NUM) return;

	//’•¶‚ª‚S‚ÂˆÈ‰º‚É‚È‚Á‚½‚ç‚P‚Â’Ç‰Á
	if (orders_.size() <= MAX_CREATE_NUM - 1)
	{
		CreateSingleOrder();
	}
}

bool OrderManager::IsFirstOrderTimeOut(void)
{
	// §ŒÀŠÔ‚ªØ‚ê‚½‚çtrue‚ğ•Ô‚·
	if (orders_.front()->GetOrderTime() < 0.1f) 
	{
		return true;
	}
	return false;
}

void OrderManager::ClearFirstOrder(void)
{
	//æ“ª‚Ì—v‘f‚ğíœ
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

Order::DRINK OrderManager::GetLastOrderDrink(void) const
{
	return orders_.back()->GetOrder().drink_;
}

Order::SWEETS OrderManager::GetLastOrderSweets(void) const
{
	return orders_.back()->GetOrder().sweets_;
}

Order::OrderData OrderManager::GetLastOrderData(void) const
{
	return orders_.back()->GetOrder();
}
