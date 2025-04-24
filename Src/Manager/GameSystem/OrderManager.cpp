#include <DxLib.h>
#include "../../Common/DebugDrawFormat.h"
#include "../../Object/Order.h"
#include "OrderManager.h"

OrderManager::OrderManager(void)
{
}

OrderManager::~OrderManager(void)
{
}

void OrderManager::Init(void)
{
	orders_.clear();
	count_ = 1;
}

void OrderManager::Update(void)
{
	// �擪�̒����̐������Ԃ��������炷
	if (!orders_.empty()) {
		orders_.front()->Update();
		// �������Ԃ��؂ꂽ�������폜
		if (orders_.front()->GetOrderTime() < 0.1f) {
			orders_.erase(orders_.begin());
			AddOrder();	//�폜�������ԍŌ�ɐV�����ǉ�
			count_++;
		}
	}
}

void OrderManager::Draw(void)
{
#ifdef _DEBUG

	auto orders = orders_.front()->GetOrder();

	//�����ɍ��킹�Ďl�p�̐F��ς���
	int startX = DebugDrawFormat::GetFormatSize("���� : %d,%d", orders.drink_, orders.sweets_);
	startX = startX * 1.5;//�t�H���g�T�C�Y��1.5�{�Ȃ̂�
	int scale = 25;
	int endX = startX + scale;
	int startY = 60;
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
		//5�܂Ő�������
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

void OrderManager::ClearOrder(void)
{
	//�擪�̗v�f���폜
	orders_.erase(orders_.begin());
}
