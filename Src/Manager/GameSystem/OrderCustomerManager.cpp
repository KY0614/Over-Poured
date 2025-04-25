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
	//����
	orderMng_ = std::make_unique<OrderManager>();
	orderMng_->Init();

	//�q
	customerMng_ = std::make_unique<CustomerManager>();
	CreateCustomersByOrders();	//��������Ă��钍�����e���Q�Ƃ��Ď�ނ����߂Đ���
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
	int line = 1;	//�s
	int lineHeight = 30;	//�s
	SetFontSize(24);
	DebugDrawFormat::FormatString("������ : %d", orderMng_->GetFirstOrder().num_, line++, lineHeight);
	DebugDrawFormat::FormatString("���� : %d,%d", orderMng_->GetFirstOrder().drink_, orderMng_->GetFirstOrder().sweets_, line++, lineHeight);
	DebugDrawFormat::FormatString("������������ : %2.f", orderMng_->GetFirstOrder().time_, line++, lineHeight);
	DebugDrawFormat::FormatString("%d��", orderMng_->GetCount(), line++, lineHeight);
	DebugDrawFormat::FormatString("%d�l", customerMng_->GetCustomerNum(), line++, lineHeight);
	DebugDrawFormat::FormatString("pos : %2.f,%2.f", customerMng_->GetFirstPos().x,customerMng_->GetFirstPos().z, line++, lineHeight);
	DebugDrawFormat::FormatString("pos : %2.f,%2.f", customerMng_->GetLastPos().x,customerMng_->GetLastPos().z, line++, lineHeight);
	SetFontSize(16);
}
