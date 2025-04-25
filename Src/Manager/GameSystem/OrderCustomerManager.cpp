#include <vector>
#include "../../Object/Customer/CustomerManager.h"
#include "./OrderManager.h"
#include "OrderCustomerManager.h"

OrderCustomerManager::OrderCustomerManager(OrderManager& orderMng):orderMng_(orderMng)
{
}

OrderCustomerManager::~OrderCustomerManager(void)
{
}

void OrderCustomerManager::Init(void)
{
	//�q
	customerMng_ = std::make_unique<CustomerManager>();
	CreateCustomerByOrder();	//��������Ă��钍�����e���Q�Ƃ��Ď�ނ����߂Đ���
	customerMng_->Init();
}

void OrderCustomerManager::Update(void)
{
	customerMng_->Update();
}

void OrderCustomerManager::Draw(void)
{
	customerMng_->Draw();
}

void OrderCustomerManager::CreateCustomerByOrder(void)
{
	std::vector<Order::DRINK> drinks = orderMng_.GetAllOrderDrink();
	for (Order::DRINK drink : drinks)
	{
		customerMng_->CreateCustomer(drink);
	}
}

void OrderCustomerManager::AddCustomerByOrder(void)
{
	Order::DRINK drink = orderMng_.GetLastOrderDrink();
	customerMng_->CreateCustomer(drink);
}

void OrderCustomerManager::ClearFirstCustomer(void)
{
	customerMng_->ClearFirstCustomers();
}

int OrderCustomerManager::GetCustomerNum(void) const
{
	return customerMng_->GetCustomerNum();
}
