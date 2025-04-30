#include <vector>
#include "../../Utility/AsoUtility.h"
#include "../../Common/DebugDrawFormat.h"
#include "../../Object/Customer/CustomerManager.h"
#include "../Generic/SceneManager.h"
#include "./OrderManager.h"
#include "OrderCustomerManager.h"

OrderCustomerManager::OrderCustomerManager(void)
{
	isCounter_ = false;
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
	customerMng_->Init();

	//生成されている注文内容を参照して種類を決めて生成
	CreateCustomersByOrders();
	customerMng_->InitCustomersPos();

	isCounter_ = false;
}

void OrderCustomerManager::Update(void)
{
	customerMng_->Update();

	//入口からカウンターまで動かす用
	if (!customerMng_->CheckFirstCustomerCol() && !isCounter_)
	{
		customerMng_->MoveCustomerPos();
		if (customerMng_->CheckFirstCustomerCol())isCounter_ = true;
	}
	
	//注文の制限時間が過ぎたら追加生成を行う
	if (orderMng_->IsFirstOrderTimeOut())
	{
		//先頭のお客と注文を削除
		ClearOrderAndCustomer();

		customerMng_->IsMoveFirstCustomer();

		////お客を一定間隔だけ移動させる
		//customerMng_->MoveCustomerPos();

		//お客の移動が終わったら削除と生成を行う
		//if (!(customerMng_->GetCustomerMove()))
		if (customerMng_->CheckFirstCustomerCol())
		{
			customerMng_->SetIsMoveFCustomer(false);
			//注文とお客を追加生成
			AddOrdersAndCustomers();
		}
	}
	else if(customerMng_->CheckFirstCustomerCol())
	{
		//制限時間を過ぎてなかったら先頭のオーダーだけ更新
		orderMng_->FirstOrderUpdate();
	}
}

void OrderCustomerManager::Draw(void)
{
	orderMng_->Draw();

	customerMng_->Draw();

#ifdef _DEBUG

	DebugDraw();

#endif // _DEBUG

}

void OrderCustomerManager::CreateCustomersByOrders(void)
{
	std::vector<Order::DRINK> drinks = orderMng_->GetAllOrderDrink();
	for (Order::DRINK drink : drinks)
	{
		customerMng_->CreateSingleCustomer(drink);
	}
}

void OrderCustomerManager::AddOrdersAndCustomers(void)
{
	//注文とお客を追加生成、位置調整
	customerMng_->SetFirstCustomerPos(customerMng_->GetFirstPos());
	orderMng_->AddOrder();
	AddCustomerByOrder();
}

void OrderCustomerManager::ClearOrderAndCustomer(void)
{
	//先頭の注文とお客を削除
	orderMng_->ClearFirstOrder();
	customerMng_->ClearFirstCustomers();
}

void OrderCustomerManager::AddCustomerByOrder(void)
{
	customerMng_->CreateSingleCustomer(orderMng_->GetLastOrderDrink());
	customerMng_->SetLastCustomerPos();
}

void OrderCustomerManager::SetAddedCustoerPos(VECTOR pos)
{
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
	DebugDrawFormat::FormatString(L"注文数 : %d", orderMng_->GetFirstOrder().num_, line++, lineHeight);
	DebugDrawFormat::FormatString(L"注文 : %d,%d", orderMng_->GetFirstOrder().drink_, orderMng_->GetFirstOrder().sweets_, line++, lineHeight);
	DebugDrawFormat::FormatString(L"注文制限時間 : %2.f", orderMng_->GetFirstOrder().time_, line++, lineHeight);
	DebugDrawFormat::FormatString(L"%d個目", orderMng_->GetCount(), line++, lineHeight);
	DebugDrawFormat::FormatString(L"%d人", customerMng_->GetCustomerNum(), line++, lineHeight);
	DebugDrawFormat::FormatString(L"pos : %2.f,%2.f", customerMng_->GetFirstPos().x,customerMng_->GetFirstPos().z, line++, lineHeight);
	DebugDrawFormat::FormatString(L"pos : %2.f,%2.f", customerMng_->GetLastPos().x,customerMng_->GetLastPos().z, line++, lineHeight);
	SetFontSize(16);
}
