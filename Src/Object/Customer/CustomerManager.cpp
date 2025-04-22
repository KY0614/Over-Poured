#include "CustomerManager.h"

CustomerManager::CustomerManager(void)
{
}

CustomerManager::~CustomerManager(void)
{
}

void CustomerManager::Init(void)
{
	for (auto& c : customers_)
	{
		c->Init();
	}
}

void CustomerManager::Update(void)
{
	for (auto& c : customers_)
	{
		c->Update();
	}
}

void CustomerManager::Draw(void)
{
	for (auto& c : customers_)
	{
		c->Draw();
	}
}

void CustomerManager::CreateCustomer(std::shared_ptr<CustomerBase> customer)
{
	customers_.push_back(customer);
}

void CustomerManager::ClearCustomers(void)
{
	customers_.pop_back();
}

void CustomerManager::CollisionCounter(void)
{
}
