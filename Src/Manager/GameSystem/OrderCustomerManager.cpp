#include <vector>
#include <algorithm>
#include "../../Utility/CommonUtility.h"
#include "../../Common/DebugDrawFormat.h"
#include "../../Manager/Generic/Camera.h"
#include "../../Object/Customer/CustomerManager.h"
#include "../../Object/Order/OrderManager.h"
#include "../../Object/Stage/StageObject.h"
#include "../Generic/SceneManager.h"
#include "OrderCustomerManager.h"

OrderCustomerManager::OrderCustomerManager(void):
	orderMng_(nullptr),
	customerMng_(nullptr),
	isServe_(false)
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
	customerMng_ = std::make_shared<CustomerManager>();

	//��������Ă��钍�����e���Q�Ƃ��Ď�ނ����߂Đ���
	CreateCustomersByOrders();
	customerMng_->Init();

	isServe_ = false;
}

void OrderCustomerManager::Update(void)
{
	//�����̍X�V
	customerMng_->Update(orderMng_->GetFirstOrder().time_);

	//��������J�E���^�[�܂œ������p
	if (!customerMng_->CheckFirstCustomerCol())
	{
		customerMng_->StartMove();
		//�������瓮���Ȃ��悤��&������\��
		if (customerMng_->CheckFirstCustomerCol())
		{
			customerMng_->StopMove();
		}
	}
	
	//�����̐������Ԃ��߂�����ǉ��������s��
	if (orderMng_->IsFirstOrderTimeOut() || isServe_)
	{
		if (!isServe_)customerMng_->SetCustomerReacton(0); isServe_ = true;

		//���q���ړ�������
		customerMng_->StartMove();

		//���q�̈ړ����I�������폜�Ɛ������s��
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
	customerMng_->Draw();
}

void OrderCustomerManager::CreateCustomersByOrders(void)
{
	std::vector<Order::OrderData> data = orderMng_->GetAllOrder();
	for(int i = 0; i < data.size();++i)
	{
		customerMng_->CreateSingleCustomer(data[i]);
	}
}

void OrderCustomerManager::AddOrdersAndCustomers(void)
{
	//�����Ƃ��q��ǉ������A�ʒu����
	orderMng_->AddCreateOrder();
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
	//���q��ǉ�����
	customerMng_->CreateSingleCustomer(orderMng_->GetLastOrderData());
}

int OrderCustomerManager::GetOrderScore(const Order::OrderData serve)
{
	Order::DRINK serveDrink = serve.drink_;
	Order::SWEETS serveSweets = serve.sweets_;
	bool serveDrinkLid = serve.lid_;
	const auto& order = orderMng_->GetFirstOrder();
	int score = 0;

	// �h�����N����
	if (order.drink_ != Order::DRINK::NONE) 
	{
		if (serve.drink_ == order.drink_) 
		{
			score += DRINK_SCORE;
			if (serve.lid_) score += LID_SCORE;
		}
		else	//�h�����N���Ⴄ�ꍇ�̓}�C�i�X
		{
			score -= DRINK_SCORE;
		}
	}
	// �X�C�[�c����
	if (order.sweets_ != Order::SWEETS::NONE) 
	{
		if (serve.sweets_ == order.sweets_) 
		{
			score += SWEETS_SCORE;
		}
		else 
		{
			score -= SWEETS_SCORE;
		}
	}
	// ���ԃ{�[�i�X
	if (order.orderNum_ == 1) 
	{
		if (order.time_ > TIME_EARLY_LIMIT) score += TIME_EARLY_BONUS;
		else if (order.time_ > TIME_LIMIT) score += TIME_BONUS;
	}
	else if (order.orderNum_ == 2) 
	{
		if (order.time_ > (TIME_EARLY_LIMIT * 2.0f)) score += TIME_EARLY_BONUS;
		else if (order.time_ > (TIME_LIMIT * 2.0f)) score += TIME_BONUS;
	}

	customerMng_->SetCustomerReacton(score);
	return score;
}

bool OrderCustomerManager::GetIsMoving(void)
{
	return customerMng_->GetIsMove();
}

Order::OrderData OrderCustomerManager::GetOrderData(void) const
{
	return orderMng_->GetFirstOrder();
}

void OrderCustomerManager::CheckServeAndOrder(const Order::OrderData serve)
{
	Order::DRINK serveDrink = serve.drink_;
	Order::SWEETS serveSweets = serve.sweets_;
	bool serveDrinkLid = serve.lid_;
	const auto& order = orderMng_->GetFirstOrder();

	const int drinkIdx = 0;
	const int sweetsIdx = 1;
	// �h�����N����
	if (order.drink_ != Order::DRINK::NONE)
	{
		if (serve.drink_ == order.drink_)
		{
			customerMng_->IsCheckUI(drinkIdx,true);
		}
		else
		{
			customerMng_->IsCheckUI(drinkIdx, false);
		}
	}
	// �X�C�[�c����
	if (order.sweets_ != Order::SWEETS::NONE)
	{
		if (serve.sweets_ == order.sweets_)
		{
			customerMng_->IsCheckUI(sweetsIdx,true);
		}
		else {
			customerMng_->IsCheckUI(sweetsIdx, false);
		}
	}
}

bool OrderCustomerManager::IsTimeOutOrder(void) const
{
	//�����̐������Ԃ��߂��Ă�����true��Ԃ�
	if (orderMng_->IsFirstOrderTimeOut())return true;
	return false;
}
