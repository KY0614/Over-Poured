#include "../../Utility/AsoUtility.h"
#include "../Order.h"
#include "../../Object/Customer/HotCustomer.h"
#include "../../Object/Customer/IceCustomer.h"
#include "CustomerManager.h"

CustomerManager::CustomerManager(void)
{
	prePos_ = AsoUtility::VECTOR_ZERO;
}

CustomerManager::~CustomerManager(void)
{
	customers_.clear();
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

void CustomerManager::InitCustomersPos(void)
{
	//とりあえず全員の位置をx軸だけずらす
	for (int i = 1; i < MAX_CREATE_SIZE; i++)
	{
		VECTOR pos = customers_.front()->GetPos();
		pos.x -= (i * CUSTOMERS_SPACE);
		customers_[i]->SetPosX(pos.x);
	}
}

void CustomerManager::CreateSingleCustomer(Order::DRINK drink)
{
	//最大注文生成数を超えそうだったらreturn
	if (customers_.size() >= MAX_CREATE_SIZE) return;
	
	switch (drink)
	{
	case Order::DRINK::NONE:
		break;

	case Order::DRINK::HOT:
		customers_.push_back(std::make_unique<HotCustomer>());
		customers_.back()->Init();
		break;

	case Order::DRINK::ICE:
		customers_.push_back(std::move(std::make_unique<IceCustomer>()));
		customers_.back()->Init();
		break;

	default:
		break;
	}
}

void CustomerManager::MoveCustomerPos(void)
{
	//customers_.front()->SetPosX(CustomerBase::COUNTER_FRONT_POS_X);

	//全員の位置をx軸だけずらす
	//for (int i = 1; i < MAX_CREATE_SIZE; i++)
	//{
	//	VECTOR pos = customers_.front()->GetPos();
	//	pos.x -= (i * CUSTOMERS_SPACE);
	//	customers_[i]->SetPosX(pos.x);
	//}

	for (auto& c : customers_)
	{
		c->Move();
	}
}

void CustomerManager::ClearFirstCustomers(void)
{
	//先頭の要素を削除
	customers_.erase(customers_.begin());
}

void CustomerManager::SetLastCustomerPos(void)
{
	//全員の位置をx軸だけずらす
	for (int i = 1; i < MAX_CREATE_SIZE; i++)
	{
		VECTOR pos = customers_.front()->GetPos();
		pos.x -= (i * CUSTOMERS_SPACE);
		customers_[i]->SetPosX(pos.x);
	}
}

bool CustomerManager::CheckFirstCustomerCol(void)
{
	bool ret = false;
	if (customers_.front()->CollisionCounter())
	{
		ret = true;
	}
	return ret;
}
