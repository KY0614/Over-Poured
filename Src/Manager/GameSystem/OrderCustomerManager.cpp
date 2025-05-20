#include <vector>
#include "../../Utility/AsoUtility.h"
#include "../../Common/DebugDrawFormat.h"
#include "../../Object/Customer/CustomerManager.h"
#include "../Generic/SceneManager.h"
#include "./OrderManager.h"
#include "OrderCustomerManager.h"

OrderCustomerManager::OrderCustomerManager(void)
{
	orderMng_ = nullptr;
	customerMng_ = nullptr;
	isServe_ = false;
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
	customerMng_ = std::make_shared<CustomerManager>();

	//��������Ă��钍�����e���Q�Ƃ��Ď�ނ����߂Đ���
	CreateCustomersByOrders();
	customerMng_->Init();
	//customerMng_->InitCustomersPos();
	isServe_ = false;
}

void OrderCustomerManager::Update(void)
{
	customerMng_->Update();

	//��������J�E���^�[�܂œ������p
	if (!customerMng_->CheckFirstCustomerCol())
	{
		customerMng_->IsMove();
		if (customerMng_->CheckFirstCustomerCol())customerMng_->IsNotMove();
	}
	
	//�����̐������Ԃ��߂�����ǉ��������s��
	if (orderMng_->IsFirstOrderTimeOut() || isServe_)
	{
		//���q���ړ�������
		customerMng_->IsMove();

		//���q�̈ړ����I�������폜�Ɛ������s��
		//if (!(customerMng_->GetCustomerMove()))
		if (customerMng_->CheckSecondCustomerCol())
		{
			//�����Ƃ��q��ǉ�����
			ClearOrderAndCustomer();

			//�ǉ�����
			AddOrdersAndCustomers();
			isServe_ = false;
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

	if(orderMng_->GetFirstOrder().num_ == 1)
	{
		if (serveDrink == orderMng_->GetFirstOrder().drink_)
		{
			score += 50;
		}
		else
		{
			score -= 100;
		}
		if (orderMng_->GetFirstOrder().time_ > 3.0f)
		{
			score += 50;
		}
		else if (orderMng_->GetFirstOrder().time_ > 1.0f)
		{
			score += 10;
		}	
		customerMng_->SetCustomerReacton(score);
	}
	else
	{
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
		customerMng_->SetCustomerReacton(score);
	}
	
	return score;
}

bool OrderCustomerManager::GetIsMoving(void)
{
	return customerMng_->GetIsMove();
}

int OrderCustomerManager::GetCustomerNum(void) const
{
	return customerMng_->GetCustomerNum();
}

void OrderCustomerManager::DebugDraw(void)
{
#ifdef _DEBUG

	int line = 1;	//�s
	int lineHeight = 30;	//�s
	SetFontSize(24);
	DebugDrawFormat::FormatString(L"���� : %d,%d", orderMng_->GetFirstOrder().drink_, orderMng_->GetFirstOrder().sweets_, line, lineHeight);
	DebugDrawFormat::FormatString(L"������������ : %2.f", orderMng_->GetFirstOrder().time_, line, lineHeight);
	DebugDrawFormat::FormatString(L"%d��", orderMng_->GetCount(), line, lineHeight);
	DebugDrawFormat::FormatString(L"%d�l", customerMng_->GetCustomerNum(), line, lineHeight);
	SetFontSize(16);

#endif // _DEBUG
}
