#include "../../Utility/AsoUtility.h"
#include "../../Object/Order/Order.h"
#include "../../Object/Customer/HotCustomer.h"
#include "../../Object/Customer/IceCustomer.h"
#include "../../Object/UI/OrderUI.h"
#include "../../Object/UI/UIManager.h"
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
	//とりあえず全員の位置をx軸だけずらす
	for (int i = 0; i < MAX_CREATE_SIZE; i++)
	{
		VECTOR pos = CustomerBase::CUSTOMER_POS;
		pos.x -= (i * CUSTOMERS_SPACE);

		customers_[i]->Init(pos);
	}

	isMove_ = true;
	cnt_ = 0;
}

void CustomerManager::Update(float orderTime)
{

	for (auto& c : customers_)
	{
		c->Update();
	}

	for (auto& ui : orderUI_)
	{
		ui->Update();
	}

	if (isMove_)
	{
		for (auto& c : customers_)
		{
			c->SetState(CustomerBase::STATE::WALK);
			c->Move();
		}
	}

	//UI座標をお客の座用と合わせる
	for (int i = 0; i < customers_.size(); ++i)
	{
		VECTOR pos = VAdd(customers_[i]->GetPos(),
			VGet(ORDER_UI_OFFSET_X, ORDER_UI_OFFSET_Y,0.0f));
		orderUI_[i]->SetPos(pos);
		orderUI_[i]->SetOrderTimer(orderTime);
	}

	//カウンターの前に来たら、回転させてカウンターの方を見るようにする
	if (customers_[cnt_]->CollisionCounter())
	{
		if (customers_[cnt_]->CheckCounterToCustomer())
		{
			customers_[cnt_]->SetGoalRotate(AsoUtility::Deg2RadF(90.0f));
			isMove_ = false;
			orderUI_[cnt_]->SetActive(true);
			for (auto& c : customers_)
			{
				c->SetState(CustomerBase::STATE::IDLE);
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
	//とりあえず全員の位置をx軸だけずらす
	for (int i = 1; i < MAX_CREATE_SIZE; i++)
	{
		VECTOR pos = customers_[cnt_]->GetPos();
		pos.x -= (i * CUSTOMERS_SPACE);
		customers_[i]->SetPosX(pos.x);
	}
}

void CustomerManager::CreateSingleCustomer(Order::OrderData data)
{
	//最大注文生成数を超えそうだったらreturn
	//if (customers_.size() >= MAX_CREATE_SIZE) return;

	orderUI_.emplace_back(std::make_unique<OrderUI>(
		data.drink_, data.sweets_,data.time_));

	switch (data.drink_)
	{
	case Order::DRINK::NONE:
		break;

	case Order::DRINK::HOT:
		customers_.emplace_back(std::make_unique<HotCustomer>());
		customers_.back()->Init(SetLastCustomerPos());
		{
			VECTOR pos = VAdd(
				SetLastCustomerPos(),
				VGet(ORDER_UI_OFFSET_X, ORDER_UI_OFFSET_Y, 0.0f));
			orderUI_.back()->Init();
			orderUI_.back()->SetPos(pos);
		}
		break;

	case Order::DRINK::ICE:
		customers_.emplace_back(std::move(std::make_unique<IceCustomer>()));
		customers_.back()->Init(SetLastCustomerPos());
		{
			VECTOR pos = VAdd(
				SetLastCustomerPos(),
				VGet(ORDER_UI_OFFSET_X, ORDER_UI_OFFSET_Y, 0.0f));
			orderUI_.back()->Init();
			orderUI_.back()->SetPos(pos);
		}
		break;

	default:
		break;
	}

	UIManager::GetInstance().AddOrderUI(orderUI_.back().get());
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
		//お客を削除
		customers_[cnt_]->IsVisible();
		orderUI_[cnt_]->SetActive(false);
		cnt_++;
	}
}

void CustomerManager::SetCustomerReacton(int score)
{
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
	//座標を返す
	VECTOR ret;

	ret = customers_[cnt_]->GetPos();
	ret.x -= ((MAX_CREATE_SIZE - 1) * CUSTOMERS_SPACE);
	return ret;
}

bool CustomerManager::CheckFirstCustomerCol(void)
{
	bool ret = false;
	//先頭のお客がカウンター前にいるかどうか
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
