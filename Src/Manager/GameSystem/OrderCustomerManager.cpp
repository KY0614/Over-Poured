#include "../../Object/Customer/CustomerManager.h"
#include "./OrderManager.h"
#include "OrderCustomerManager.h"

OrderCustomerManager::OrderCustomerManager(void)
{
}

OrderCustomerManager::~OrderCustomerManager(void)
{
}

void OrderCustomerManager::Init(void)
{
	//�q
	customerMng_ = std::make_unique<CustomerManager>();
	customerMng_->Init();

	//����
	orderMng_ = std::make_unique<OrderManager>();
	orderMng_->Init();

}

void OrderCustomerManager::Update(void)
{
}

void OrderCustomerManager::Draw(void)
{
}

void OrderCustomerManager::CreateCustomerByOrder(void)
{
}
