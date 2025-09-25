#include <DxLib.h>
#include "Order.h"
#include "OrderManager.h"

namespace 
{
	//�����֘A
	const int MAX_CREATE_NUM = 6;	//�ő咍��������
}

OrderManager::OrderManager(void)
{
	orders_.clear();
}

OrderManager::~OrderManager(void)
{
	orders_.clear();
}

void OrderManager::Init(void)
{
	//�z��̏�����
	orders_.clear();

	//�������ő吔�܂Ő�������
	CreateOrderMaxNum();
}

void OrderManager::FirstOrderUpdate(void)
{
	//orders�̒��g���󂾂�����X�V���Ȃ�
	if (orders_.empty()) return;

	//�擪�̒����̐������Ԃ����炷
	orders_.front()->TimerUpdate();
}

void OrderManager::AddCreateOrder(void)
{
	//�ő咍���������𒴂������������珈�����Ȃ�
	if (orders_.size() >= MAX_CREATE_NUM) return;

	//�������ő吔�����ɂȂ�����P�ǉ�
	if (orders_.size() < MAX_CREATE_NUM)
	{
		CreateSingleOrder();
	}
}

void OrderManager::ClearFirstOrder(void)
{
	//�擪�̗v�f���폜
	orders_.erase(orders_.begin());
}

const bool& OrderManager::IsFirstOrderTimeOut(void)
{
	//�������Ԃ��؂ꂽ��true��Ԃ�
	if (orders_.front()->GetOrderTime() < 0.1f)	//0.0f�ȉ����ƌ덷�Ń}�C�i�X�ɂȂ�\�������邽��0.1f�ȉ��ɂ��Ă���
	{
		return true;
	}
	return false;
}

const std::vector<Order::OrderData>& OrderManager::GetAllOrder(void) const
{
	//���݂̑S�Ă̒����f�[�^��Ԃ�
	std::vector<Order::OrderData> retData;
	for (const auto& order : orders_)
	{
		retData.push_back(order->GetOrder());
	}
	return retData;
}

const Order::OrderData& OrderManager::GetLastOrderData(void) const
{
	//�Ō�̒����f�[�^��Ԃ�
	return orders_.back()->GetOrder();
}

void OrderManager::CreateOrderMaxNum(void)
{
	//���������łɐ�������Ă����琶�����Ȃ�
	if (orders_.empty())
	{
		//�ő吔�܂Ő�������
		for (int i = 0; i < MAX_CREATE_NUM; i++)
		{
			//�P����
			CreateSingleOrder();
		}
	}
}

void OrderManager::CreateSingleOrder(void)
{
	//�ő咍���������𒴂������������琶�����Ȃ�
	if (orders_.size() >= MAX_CREATE_NUM) return;

	//�����𐶐�����
	std::unique_ptr<Order> order = std::make_unique<Order>();
	order->CreateOrder();		//�����𐶐�
	orders_.push_back(std::move(order));
}