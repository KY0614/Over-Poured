#include "CustomerManager.h"

CustomerManager* CustomerManager::instance_ = nullptr;

void CustomerManager::CreateInstance(void)
{
	if (instance_ == nullptr)
	{
		instance_ = new CustomerManager();
	}
	instance_->Init();
}

CustomerManager& CustomerManager::GetInstance(void)
{
	if (instance_ == nullptr)
	{
		CustomerManager::CreateInstance();
	}
	return *instance_;
}

void CustomerManager::Init(void)
{
}

void CustomerManager::Update(void)
{
}

void CustomerManager::Draw(void)
{
}

void CustomerManager::Destroy(void)
{
	delete instance_;
}

void CustomerManager::CreateCustomer(OrderManager& order)
{
	if (order.GetInstance().GetOrder().drink_ == OrderManager::DRINK::HOT)
	{

	}
}
