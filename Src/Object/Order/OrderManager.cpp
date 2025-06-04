#include <DxLib.h>
#include "../../Common/DebugDrawFormat.h"
#include "Order.h"
#include "OrderManager.h"

OrderManager::OrderManager(void)
{
	//count_ = 0;
}

OrderManager::~OrderManager(void)
{
	orders_.clear();
}

void OrderManager::Init(void)
{
	orders_.clear();
	InitOrder();
	//count_ = 1;
}

void OrderManager::FirstOrderUpdate(void)
{
	//orders�̒��g���󂾂����炵�Ȃ�
	if (orders_.empty()) return;

	//�擪�̒�����\�����A�������Ԃ��������炷
	orders_.front()->Update();
}

void OrderManager::Draw(void)
{
#ifdef _DEBUG

	auto orders = orders_.front()->GetOrder();

	//�����ɍ��킹�Ďl�p�̐F��ς���
	int startX = DebugDrawFormat::GetFormatSize(L"���� : %d,%d", orders.drink_, orders.sweets_);
	startX = startX * 1.5;//�t�H���g�T�C�Y��1.5�{�Ȃ̂�
	int scale = 25;
	int endX = startX + scale;
	int startY = 30;
	int endY = startY + scale;
	int drinkCol = GetColor(0, 0, 0);

	if (orders.drink_ == Order::DRINK::HOT)
	{
		drinkCol = GetColor(255, 0, 0);
	}
	else
	{
		drinkCol = GetColor(0, 255, 255);
	}
	//���ݕ��p
	DrawBox(startX, startY, endX, endY, drinkCol, true);

	int foodCol = GetColor(0, 0, 0);
	switch (orders.sweets_)
	{
	case Order::SWEETS::NONE:
		foodCol = GetColor(0, 0, 0);
		break;

	case Order::SWEETS::CHOCO:
		foodCol = GetColor(132, 98, 68);
		break;

	case Order::SWEETS::STRAWBERRY:
		foodCol = GetColor(255, 198, 244);
		break;
	default:
		break;
	}

	//�H�ו��p
	DrawBox(endX + scale, startY, endX + (scale * 2), endY, foodCol, true);
#endif // _DEBUG
}

void OrderManager::InitOrder(void)
{
	if (orders_.empty())
	{
		//6�܂Ő�������
		for (int i = 0; i < MAX_CREATE_NUM; i++)
		{
			CreateSingleOrder();
		}
	}
}

void OrderManager::CreateSingleOrder(void)
{
	//�ő咍���������𒴂�������������return
	if (orders_.size() >= MAX_CREATE_NUM) return;

	std::unique_ptr<Order> order = std::make_unique<Order>();
	order->CreateOrder();		//�����𐶐�
	orders_.push_back(std::move(order));
}

void OrderManager::AddOrder(void)
{
	//�ő咍���������𒴂�������������return
	if (orders_.size() >= MAX_CREATE_NUM) return;

	//�������S�ȉ��ɂȂ�����P�ǉ�
	if (orders_.size() <= MAX_CREATE_NUM - 1)
	{
		CreateSingleOrder();
	}
}

bool OrderManager::IsFirstOrderTimeOut(void)
{
	// �������Ԃ��؂ꂽ��true��Ԃ�
	if (orders_.front()->GetOrderTime() < 0.1f) 
	{
		return true;
	}
	return false;
}

void OrderManager::ClearFirstOrder(void)
{
	//�擪�̗v�f���폜
	orders_.erase(orders_.begin());
	//count_++;
}

std::vector<Order::DRINK> OrderManager::GetAllOrderDrink(void) const
{
	std::vector<Order::DRINK> drink;
	for (auto& order : orders_)
	{
		drink.push_back(order->GetOrder().drink_);
	}

	return drink;
}

Order::DRINK OrderManager::GetLastOrderDrink(void) const
{
	return orders_.back()->GetOrder().drink_;
}
