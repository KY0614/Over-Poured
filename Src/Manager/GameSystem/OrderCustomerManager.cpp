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
	//íçï∂
	orderMng_ = std::make_unique<OrderManager>();
	orderMng_->Init();

	//ãq
	customerMng_ = std::make_unique<CustomerManager>();
	customerMng_->Init();

	//ê∂ê¨Ç≥ÇÍÇƒÇ¢ÇÈíçï∂ì‡óeÇéQè∆ÇµÇƒéÌóﬁÇåàÇﬂÇƒê∂ê¨
	CreateCustomersByOrders();
	customerMng_->InitCustomersPos();

	isCounter_ = false;
}

void OrderCustomerManager::Update(void)
{
	customerMng_->Update();

	if (!customerMng_->CheckFirstCustomerCol() && !isCounter_)
	{
		customerMng_->MoveCustomerPos();
		if (customerMng_->CheckFirstCustomerCol())isCounter_ = true;
	}
	

	//íçï∂ÇÃêßå¿éûä‘Ç™âﬂÇ¨ÇΩÇÁí«â¡ê∂ê¨ÇçsÇ§
	if (orderMng_->IsFirstOrderTimeOut())
	{
		//Ç®ãqÇàÍíËä‘äuÇæÇØà⁄ìÆÇ≥ÇπÇÈ
		customerMng_->MoveCustomerPos();

		//Ç®ãqÇÃà⁄ìÆÇ™èIÇÌÇ¡ÇΩÇÁçÌèúÇ∆ê∂ê¨ÇçsÇ§
		if (!(customerMng_->GetCustomerMove()))
		{
			//êÊì™ÇÃÇ®ãqÇ∆íçï∂ÇçÌèú
			ClearOrderAndCustomer();

			//íçï∂Ç∆Ç®ãqÇí«â¡ê∂ê¨
			AddOrdersAndCustomers();
		}
	}
	else if(customerMng_->CheckFirstCustomerCol())
	{
		//êßå¿éûä‘ÇâﬂÇ¨ÇƒÇ»Ç©Ç¡ÇΩÇÁêÊì™ÇÃÉIÅ[É_Å[ÇæÇØçXêV
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
	//íçï∂Ç∆Ç®ãqÇí«â¡ê∂ê¨ÅAà íuí≤êÆ
	customerMng_->SetFirstCustomerPos(customerMng_->GetFirstPos());
	orderMng_->AddOrder();
	AddCustomerByOrder();
}

void OrderCustomerManager::ClearOrderAndCustomer(void)
{
	//êÊì™ÇÃíçï∂Ç∆Ç®ãqÇçÌèú
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
	int line = 1;	//çs
	int lineHeight = 30;	//çs
	SetFontSize(24);
	DebugDrawFormat::FormatString(L"íçï∂êî : %d", orderMng_->GetFirstOrder().num_, line++, lineHeight);
	DebugDrawFormat::FormatString(L"íçï∂ : %d,%d", orderMng_->GetFirstOrder().drink_, orderMng_->GetFirstOrder().sweets_, line++, lineHeight);
	DebugDrawFormat::FormatString(L"íçï∂êßå¿éûä‘ : %2.f", orderMng_->GetFirstOrder().time_, line++, lineHeight);
	DebugDrawFormat::FormatString(L"%då¬ñ⁄", orderMng_->GetCount(), line++, lineHeight);
	DebugDrawFormat::FormatString(L"%dêl", customerMng_->GetCustomerNum(), line++, lineHeight);
	DebugDrawFormat::FormatString(L"pos : %2.f,%2.f", customerMng_->GetFirstPos().x,customerMng_->GetFirstPos().z, line++, lineHeight);
	DebugDrawFormat::FormatString(L"pos : %2.f,%2.f", customerMng_->GetLastPos().x,customerMng_->GetLastPos().z, line++, lineHeight);
	SetFontSize(16);
}
