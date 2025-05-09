#include "../../Utility/AsoUtility.h"
#include "../Order.h"
#include "../../Object/Customer/HotCustomer.h"
#include "../../Object/Customer/IceCustomer.h"
#include "CustomerManager.h"

CustomerManager::CustomerManager(void)
{
	isMove_ = false;
	cnt_ = 0;
}

CustomerManager::~CustomerManager(void)
{
	customers_.clear();
}

void CustomerManager::Init(void)
{
	//�Ƃ肠�����S���̈ʒu��x���������炷
	for (int i = 0; i < MAX_CREATE_SIZE; i++)
	{
		VECTOR pos = CustomerBase::CUSTOMER_POS;
		pos.x -= (i * CUSTOMERS_SPACE);

		customers_[i]->Init(pos);
	}

	isMove_ = true;
	cnt_ = 0;
	
	//for (auto& c : customers_)
	//{
	//	c->Init();
	//}
}

void CustomerManager::Update(void)
{
	if (cnt_ < customers_.size())return;
	for (auto& c : customers_)
	{
		c->Update();
	}

	//if (customers_.front()->GetIsMove() && !(customers_.front()->CollisionCounter()))
	//{
	//	for (auto& c : customers_)
	//	{
	//		c->Move();
	//	}
	//}

	if (isMove_)
	{
		for (auto& c : customers_)
		{
			c->SetState(CustomerBase::STATE::WALK);
			c->Move();
		}
	}
	
	//�J�E���^�[�O�̓����蔻��̈ʒu�܂œ������A��]������
	//if (customers_.front()->CollisionCounter())
	//{
	//	if (customers_.front()->CheckCounterToCustomer())
	//	{
	//		customers_.front()->SetGoalRotate(AsoUtility::Deg2RadF(90.0f));
	//		isMove_ = false;
	//		for (auto& c : customers_)
	//		{
	//			c->SetState(CustomerBase::STATE::WAIT);
	//		}
	//	}
	//}

	if (customers_[cnt_]->CollisionCounter())
	{
		if (customers_[cnt_]->CheckCounterToCustomer())
		{
			customers_[cnt_]->SetGoalRotate(AsoUtility::Deg2RadF(90.0f));
			isMove_ = false;
			for (auto& c : customers_)
			{
				c->SetState(CustomerBase::STATE::WAIT);
			}
		}
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
	//�Ƃ肠�����S���̈ʒu��x���������炷
	for (int i = 1; i < MAX_CREATE_SIZE; i++)
	{
		VECTOR pos = customers_[cnt_]->GetPos();
		pos.x -= (i * CUSTOMERS_SPACE);
		customers_[i]->SetPosX(pos.x);
	}
}

void CustomerManager::CreateSingleCustomer(Order::DRINK drink)
{
	//�ő咍���������𒴂�������������return
	//if (customers_.size() >= MAX_CREATE_SIZE) return;
	
	switch (drink)
	{
	case Order::DRINK::NONE:
		break;

	case Order::DRINK::HOT:
		customers_.emplace_back(std::make_unique<HotCustomer>());
		customers_.back()->Init(SetLastCustomerPos());

		//customers_[MAX_CREATE_SIZE - 1] = std::make_unique<HotCustomer>();
		//customers_[MAX_CREATE_SIZE - 1]->Init(SetLastCustomerPos());
		break;

	case Order::DRINK::ICE:
		customers_.emplace_back(std::move(std::make_unique<IceCustomer>()));
		customers_.back()->Init(SetLastCustomerPos());

		//customers_[MAX_CREATE_SIZE - 1] = std::make_unique<HotCustomer>();
		//customers_[MAX_CREATE_SIZE - 1]->Init(SetLastCustomerPos());
		break;

	default:
		break;
	}

}

void CustomerManager::MoveCustomerPos(void)
{
	for (auto& c : customers_)
	{
		c->Move();
	}
}

void CustomerManager::ClearFirstCustomers(void)
{
	if (!customers_.empty())
	{
		////�擪�̗v�f���폜
		//std::unique_ptr<CustomerBase> ptr = std::move(customers_.front());  //  ���L������
		//customers_.erase(customers_.begin());
		
		//customers_.front()->IsVisible();
		customers_[cnt_++]->IsVisible();
	}
}

void CustomerManager::SetCustomerReacton(int score)
{
	//if (score >= 80)
	//{
	//	customers_.front()->SetReaction(CustomerBase::REACTION::GOOD);
	//}
	//else if (score > 50)
	//{
	//	customers_.front()->SetReaction(CustomerBase::REACTION::SOSO);
	//}
	//else
	//{
	//	customers_.front()->SetReaction(CustomerBase::REACTION::BAD);
	//}

	if (score >= 80)
	{
		customers_[cnt_]->SetReaction(CustomerBase::REACTION::GOOD);
	}
	else if (score > 50)
	{
		customers_[cnt_]->SetReaction(CustomerBase::REACTION::SOSO);
	}
	else
	{
		customers_[cnt_]->SetReaction(CustomerBase::REACTION::BAD);
	}
}

VECTOR CustomerManager::SetLastCustomerPos(void)
{
	//���W��Ԃ�
	VECTOR ret;

	//ret = customers_.front()->GetPos();
	ret = customers_[cnt_]->GetPos();
	ret.x -= ((MAX_CREATE_SIZE - 1) * CUSTOMERS_SPACE);
	return ret;
}

bool CustomerManager::CheckFirstCustomerCol(void)
{
	bool ret = false;
	//if (customers_.front()->CollisionCounter())
	if (customers_[cnt_]->CollisionCounter())
	{
		ret = true;
	}
	return ret;
}

bool CustomerManager::CheckSecondCustomerCol(void)
{
	bool ret = false;
	if (customers_[cnt_ + 1]->CollisionCounter())
	{
		ret = true;
	}
	return ret;
}
