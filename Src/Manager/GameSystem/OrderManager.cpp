#include <DxLib.h>
#include "../../Object/Order.h"
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
	orders_.clear();
}

void OrderManager::Update(void)
{
	// æ“ª‚Ì’•¶‚Ì§ŒÀŠÔ‚¾‚¯‚ğŒ¸‚ç‚·
	if (!orders_.empty()) {
		orders_.front()->Update();
		// §ŒÀŠÔ‚ªØ‚ê‚½’•¶‚ğíœ
		if (orders_.front()->GetOrderTime() < 0.1f) {
			orders_.erase(orders_.begin());
			CreateOrder();	//íœ‚µ‚½‚çˆê”ÔÅŒã‚ÉV‚µ‚­’Ç‰Á
		}
	}
}

void OrderManager::Destroy(void)
{
	delete instance_;
	orders_.clear();
}

void OrderManager::InitCreateOrder(void)
{
	//5‚Â‚Ü‚Å¶¬‚·‚é
	for (int i = 0; i < MAX_CREATE_NUM; i++)
	{
		orders_.push_back(orders_[i]);
		orders_[i]->CreateOrder();
	}
}

void OrderManager::CreateOrder(void)
{
	if (orders_.size() >= MAX_CREATE_NUM) return;

	std::unique_ptr<Order> order = nullptr;
	order = std::make_unique<Order>();

	//5‚Â‚Ü‚Å¶¬‚·‚é
	for (int i = 0; i < MAX_CREATE_NUM; i++)
	{
		orders_.push_back(std::move(order));
		orders_[i]->CreateOrder();
	}

	int num = MAX_CREATE_NUM - 1;
	if (orders_.size() <= MAX_CREATE_NUM - 1)
	{
		std::unique_ptr<Order> newOrder = nullptr;
		newOrder = std::make_unique<Order>();

		orders_.push_back(std::move(newOrder));
		orders_[num]->CreateOrder();
	}
}

void OrderManager::AddOrder(std::unique_ptr<Order> order)
{
	orders_.push_back(std::move(order));
}

void OrderManager::ClearOrder(void)
{
	//æ“ª‚Ì—v‘f‚ğíœ
	orders_.erase(orders_.begin());
}
