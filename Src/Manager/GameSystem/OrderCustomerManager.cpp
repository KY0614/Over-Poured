#include <vector>
#include "../../Utility/AsoUtility.h"
#include "../../Common/DebugDrawFormat.h"
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
	//注文
	orderMng_ = std::make_unique<OrderManager>();
	orderMng_->Init();

	//客
	customerMng_ = std::make_unique<CustomerManager>();
	CreateCustomersByOrders();	//生成されている注文内容を参照して種類を決めて生成
	customerMng_->Init();
}

void OrderCustomerManager::Update(void)
{
	orderMng_->FirstOrderUpdate();

	customerMng_->Update();

	AddOrdersAndCustomers();
}

void OrderCustomerManager::Draw(void)
{
	orderMng_->Draw();

	customerMng_->Draw();

	DebugDraw();
}

void OrderCustomerManager::CreateCustomersByOrders(void)
{
	std::vector<Order::DRINK> drinks = orderMng_->GetAllOrderDrink();
	for (Order::DRINK drink : drinks)
	{
		customerMng_->CreateCustomer(drink);
	}
}

void OrderCustomerManager::AddOrdersAndCustomers(void)
{
	if (orderMng_->IsFirstOrderTimeOut())
	{
		orderMng_->ClearFirstOrder();
		customerMng_->SetFirstPos(customerMng_->GetFirstPos());
		customerMng_->ClearFirstCustomers();

		orderMng_->AddOrder();
		AddCustomerByOrder();
		customerMng_->Move2PrePos();
	}
	else { return; }
}

void OrderCustomerManager::AddCustomerByOrder(void)
{
	customerMng_->CreateCustomer(orderMng_->GetLastOrderDrink());
}

int OrderCustomerManager::GetCustomerNum(void) const
{
	return customerMng_->GetCustomerNum();
}

void OrderCustomerManager::DebugDraw(void)
{
	int line = 1;	//行
	int lineHeight = 30;	//行
	SetFontSize(24);
	DebugDrawFormat::FormatString("注文数 : %d", orderMng_->GetFirstOrder().num_, line++, lineHeight);
	DebugDrawFormat::FormatString("注文 : %d,%d", orderMng_->GetFirstOrder().drink_, orderMng_->GetFirstOrder().sweets_, line++, lineHeight);
	DebugDrawFormat::FormatString("注文制限時間 : %2.f", orderMng_->GetFirstOrder().time_, line++, lineHeight);
	DebugDrawFormat::FormatString("%d個目", orderMng_->GetCount(), line++, lineHeight);
	DebugDrawFormat::FormatString("%d人", customerMng_->GetCustomerNum(), line++, lineHeight);
	DebugDrawFormat::FormatString("pos : %2.f,%2.f", customerMng_->GetFirstPos().x,customerMng_->GetFirstPos().z, line++, lineHeight);
	DebugDrawFormat::FormatString("pos : %2.f,%2.f", customerMng_->GetLastPos().x,customerMng_->GetLastPos().z, line++, lineHeight);
	SetFontSize(16);
}
