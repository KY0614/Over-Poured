#include <DxLib.h>
#include "../../Object/Order.h"
#include "OrderManager.h"

OrderManager* OrderManager::instance_ = nullptr;

void OrderManager::CreateInstance(void)
{
	if (instance_ == nullptr)
	{
		instance_ = new OrderManager();
	}
	instance_->Init();
}

OrderManager& OrderManager::GetInstance(void)
{
	if (instance_ == nullptr)
	{
		OrderManager::CreateInstance();
	}
	return *instance_;
}

void OrderManager::Init(void)
{
	orders_.clear();
}

void OrderManager::Update(void)
{
	// �擪�̒����̐������Ԃ��������炷
	if (!orders_.empty()) {
		orders_.front()->Update();
		// �������Ԃ��؂ꂽ�������폜
		if (orders_.front()->GetOrderTime() < 0.1f) {
			orders_.erase(orders_.begin());
			CreateOrder();	//�폜�������ԍŌ�ɐV�����ǉ�
		}
	}
}

void OrderManager::Destroy(void)
{
	delete instance_;
	orders_.clear();
}

void OrderManager::InitCreateOrder(void)
{
	//5�܂Ő�������
	for (int i = 0; i < MAX_CREATE_NUM; i++)
	{
		orders_.push_back(orders_[i]);
		orders_[i]->CreateOrder();
	}
}

void OrderManager::CreateOrder(void)
{
	if (orders_.size() >= MAX_CREATE_NUM) return;

	std::unique_ptr<Order> order = nullptr;
	order = std::make_unique<Order>();

	//5�܂Ő�������
	for (int i = 0; i < MAX_CREATE_NUM; i++)
	{
		orders_.push_back(std::move(order));
		orders_[i]->CreateOrder();
	}

	int num = MAX_CREATE_NUM - 1;
	if (orders_.size() <= MAX_CREATE_NUM - 1)
	{
		std::unique_ptr<Order> newOrder = nullptr;
		newOrder = std::make_unique<Order>();

		orders_.push_back(std::move(newOrder));
		orders_[num]->CreateOrder();
	}
}

void OrderManager::AddOrder(std::unique_ptr<Order> order)
{
	orders_.push_back(std::move(order));
}

void OrderManager::ClearOrder(void)
{
	//�擪�̗v�f���폜
	orders_.erase(orders_.begin());
}
