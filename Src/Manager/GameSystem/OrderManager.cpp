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
}

void OrderManager::Update(void)
{
}

void OrderManager::Destroy(void)
{
}
