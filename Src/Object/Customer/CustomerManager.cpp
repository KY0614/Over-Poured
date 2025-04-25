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
}

void CustomerManager::Update(void)
{
	for (auto& c : customers_)
	{
		c->Update();
	}

	//�Ƃ肠�����S���̈ʒu��x���������炷
	for (int i = 0; i < MAX_CREATE_SIZE; i++)
	{
		VECTOR pos = customers_.front()->GetPos();
		pos.x -= (i * 80.0f);
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

void CustomerManager::InitFiveCustomer(Order::DRINK drink)
{
	//if (customers_.empty())
	//{
		////5�l�܂Ő�������
		//for (int i = 0; i < MAX_CREATE_SIZE; i++)
		//{
			CreateCustomer(drink);
		//}
	//}
}

void CustomerManager::CreateCustomer(Order::DRINK drink)
{
	//�ő咍���������𒴂�������������return
	if (customers_.size() >= MAX_CREATE_SIZE) return;
	
	switch (drink)
	{
	case Order::DRINK::NONE:
		break;

	case Order::DRINK::HOT:
		customers_.push_back(std::make_unique<HotCustomer>());
		break;

	case Order::DRINK::ICE:
		customers_.push_back(std::move(std::make_unique<IceCustomer>()));
		break;

	default:
		break;
	}
}

void CustomerManager::ClearFirstCustomers(void)
{
	//�擪�̗v�f���폜
	customers_.erase(customers_.begin());
}

void CustomerManager::CollisionCounter(void)
{
}

void CustomerManager::Move2PrePos(void)
{
	//�Ƃ肠�����S���̈ʒu��x���������炷
	for (int i = 1; i < MAX_CREATE_SIZE; i++)
	{
		float preposX = prePos_.x;
		preposX -= (i * 80.0f);
		customers_[i]->SetPos(preposX);
	}
}
