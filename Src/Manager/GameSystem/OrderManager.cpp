#include <DxLib.h>
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
	orderNum_ = 0;
}

void OrderManager::Update(void)
{
}

void OrderManager::Destroy(void)
{
	delete instance_;
}

OrderManager::Order OrderManager::CreateOrder(void)
{
	int drinkType = 0;
	int sweetsType = 0;

	//�����������߂�
	do
	{
		orderNum_ = GetRand(ORDER_MAX_NUM);
	} while (orderNum_ == 0);	//0�������痐���擾���Ȃ���

	//�������e�����߂�
	switch (orderNum_)
	{
	case 1:
		do
		{
			drinkType = GetRand(DRINK_MAX_NUM);
		} while (drinkType == 0);	//0�������痐���擾���Ȃ���

		sweetsType = 0;		//���������P�̎��̓X�C�[�c�͗��܂Ȃ��悤�ɂ���
		SetDrink(static_cast<DRINK>(drinkType));	//���ݕ��̎�ނ�ݒ�
		SetSweets(static_cast<SWEETS>(sweetsType));	//�H�ו��̎�ނ�ݒ�

		SetOrderTime(ONE_ORDER_TIME);				//�������Ԃ�ݒ�
		break;

	case 2:

		do
		{
			drinkType = GetRand(DRINK_MAX_NUM);
		} while (drinkType == 0);	//0�������痐���擾���Ȃ���

		do
		{
			sweetsType = GetRand(FOOD_MAX_NUM);
		} while (sweetsType == 0);	//0�������痐���擾���Ȃ���

		SetDrink(static_cast<DRINK>(drinkType));	//���ݕ��̎�ނ�ݒ�
		SetSweets(static_cast<SWEETS>(sweetsType));	//�H�ו��̎�ނ�ݒ�

		SetOrderTime(TWO_ORDER_TIME);				//�������Ԃ�ݒ�
		break;

	default:
		break;
	}

	return order_;
}
