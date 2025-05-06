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
	orderMng_ = nullptr;
	customerMng_ = nullptr;
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

	//��������Ă��钍�����e���Q�Ƃ��Ď�ނ����߂Đ���
	CreateCustomersByOrders();
	customerMng_->Init();
	//customerMng_->InitCustomersPos();

	isCounter_ = false;
}

void OrderCustomerManager::Update(void)
{
	customerMng_->Update();

	//��������J�E���^�[�܂œ������p
	if (!customerMng_->CheckFirstCustomerCol() && !isCounter_)
	{
		customerMng_->MoveCustomerPos();
		if (customerMng_->CheckFirstCustomerCol())isCounter_ = true;
	}
	
	//�����̐������Ԃ��߂�����ǉ��������s��
	if (orderMng_->IsFirstOrderTimeOut())
	{
		//�擪�̂��q�ƒ������폜
		ClearOrderAndCustomer();

		customerMng_->IsMoveFirstCustomer();

		//�ǉ�����
		AddOrdersAndCustomers();
		
		////���q�����Ԋu�����ړ�������
		//customerMng_->MoveCustomerPos();

		//���q�̈ړ����I�������폜�Ɛ������s��
		//if (!(customerMng_->GetCustomerMove()))
		if (customerMng_->CheckFirstCustomerCol())
		{
			//customerMng_->SetIsMoveFCustomer(false);
			//�����Ƃ��q��ǉ�����
			
		}
	}
	else if(customerMng_->CheckFirstCustomerCol())
	{
		//�������Ԃ��߂��ĂȂ�������擪�̃I�[�_�[�����X�V
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
	//�����Ƃ��q��ǉ������A�ʒu����
	//customerMng_->SetFirstCustomerPos(customerMng_->GetFirstPos());
	orderMng_->AddOrder();
	AddCustomerByOrder();
}

void OrderCustomerManager::ClearOrderAndCustomer(void)
{
	//�擪�̒����Ƃ��q���폜
	orderMng_->ClearFirstOrder();
	customerMng_->ClearFirstCustomers();
}

void OrderCustomerManager::AddCustomerByOrder(void)
{
	customerMng_->CreateSingleCustomer(orderMng_->GetLastOrderDrink());
	customerMng_->SetLastCustomerPos();
}

int OrderCustomerManager::CheckServeAndOrder(Order::OrderData serve)
{
	Order::DRINK serveDrink = serve.drink_;
	Order::SWEETS serveSweets = serve.sweets_;
	int score = 0;
	if (serveDrink == orderMng_->GetFirstOrder().drink_)
	{
		score += 30;
	}

	if (serveSweets == orderMng_->GetFirstOrder().sweets_)
	{
		score += 30;
	}

	if (orderMng_->GetFirstOrder().time_ > 2.0f)
	{
		score += 40;
	}
	else if (orderMng_->GetFirstOrder().time_ > 1.0f)
	{
		score += 20;
	}

	return score;
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
	DebugDrawFormat::FormatString(L"������ : %d", orderMng_->GetFirstOrder().num_, line++, lineHeight);
	DebugDrawFormat::FormatString(L"���� : %d,%d", orderMng_->GetFirstOrder().drink_, orderMng_->GetFirstOrder().sweets_, line++, lineHeight);
	DebugDrawFormat::FormatString(L"������������ : %2.f", orderMng_->GetFirstOrder().time_, line++, lineHeight);
	DebugDrawFormat::FormatString(L"%d��", orderMng_->GetCount(), line++, lineHeight);
	DebugDrawFormat::FormatString(L"%d�l", customerMng_->GetCustomerNum(), line++, lineHeight);
	DebugDrawFormat::FormatString(L"pos : %2.f,%2.f", customerMng_->GetFirstPos().x,customerMng_->GetFirstPos().z, line++, lineHeight);
	DebugDrawFormat::FormatString(L"pos : %2.f,%2.f", customerMng_->GetSecondPos().x,customerMng_->GetSecondPos().z, line++, lineHeight);
	DebugDrawFormat::FormatString(L"pos : %2.f,%2.f", customerMng_->GetSecondPos().x - customerMng_->GetFirstPos().x,customerMng_->GetLastPos().z, line++, lineHeight);
	SetFontSize(16);
}
