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
	//customers_.clear();

	for (auto& c : customers_)
	{
		c->Init();
	}

	//とりあえず全員の位置をx軸だけずらす
	for (int i = 0; i < MAX_CREATE_SIZE; i++)
	{
		VECTOR pos = customers_.front()->GetPos();
		pos.x -= (i * 80.0f);
		customers_[i]->GetPos();
		customers_[i]->SetPos(pos.x);
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

void CustomerManager::InitFiveCustomer(Order::DRINK drink)
{
	//if (customers_.empty())
	//{
		////5人まで生成する
		//for (int i = 0; i < MAX_CREATE_SIZE; i++)
		//{
			CreateCustomer(drink);
		//}
	//}
}

void CustomerManager::CreateCustomer(Order::DRINK drink)
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

void CustomerManager::ClearFirstCustomers(void)
{
	//先頭の要素を削除
	customers_.erase(customers_.begin());
}

void CustomerManager::CollisionCounter(void)
{
}

void CustomerManager::Move2PrePos(void)
{
	//とりあえず全員の位置をx軸だけずらす
	for (int i = 0; i < MAX_CREATE_SIZE; i++)
	{
		customers_[i]->SetPos(prePos_.x);
		prePos_.x -= (i * 80.0f);
		
	}
}
