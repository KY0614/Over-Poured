#include "../../Utility/AsoUtility.h"
#include "../Order.h"
#include "../../Object/Customer/HotCustomer.h"
#include "../../Object/Customer/IceCustomer.h"
#include "CustomerManager.h"

CustomerManager::CustomerManager(void)
{
}

CustomerManager::~CustomerManager(void)
{
	customers_.clear();
}

void CustomerManager::Init(void)
{
	customers_.clear();
	InitCustomer();

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

	//とりあえず全員の位置をx軸だけずらす
	for (int i = 0; i < MAX_CREATE_SIZE; i++)
	{
		VECTOR pos = AsoUtility::VECTOR_ZERO;
		pos.x += -100.0f + (i * 80.0f);
		customers_[i]->GetPos();
		customers_[i]->SetPos(pos.x);
	}
}

void CustomerManager::Draw(void)
{
	for (auto& c : customers_)
	{
		c->Draw();
	}
}

void CustomerManager::InitCustomer(void)
{
	if (customers_.empty())
	{
		//5人まで生成する
		for (int i = 0; i < MAX_CREATE_SIZE; i++)
		{
			CreateCustomer(Order::DRINK::HOT);
		}
	}
}

void CustomerManager::CreateCustomer(Order::DRINK order)
{
	//最大注文生成数を超えそうだったらreturn
	if (customers_.size() >= MAX_CREATE_SIZE) return;

	//std::unique_ptr<CustomerBase> hot = ;
	std::unique_ptr<CustomerBase> ice = std::make_unique<IceCustomer>();
	
	switch (order)
	{
	case Order::DRINK::NONE:
		break;

	case Order::DRINK::HOT:
		customers_.push_back(std::make_unique<HotCustomer>());
		break;

	case Order::DRINK::ICE:
		customers_.push_back(std::move(ice));
		break;

	default:
		break;
	}
}

void CustomerManager::ClearCustomers(void)
{
	//先頭の要素を削除
	customers_.erase(customers_.begin());
}

void CustomerManager::CollisionCounter(void)
{
}
