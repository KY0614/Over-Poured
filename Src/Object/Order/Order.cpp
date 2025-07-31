#include <DxLib.h>
#include "../Libs/ImGui/imgui.h"
#include "../Common/DebugDrawFormat.h"
#include "../Manager/Generic/SceneManager.h"
#include "../Manager/Generic/InputManager.h"
#include "OrderManager.h"
#include "Order.h"

Order::Order(void):
	orderData_(),
	orderNum_(-1)
{
}

Order::~Order(void)
{
}

void Order::TimerUpdate(void)
{
	//�������Ԃ���o�ߎ��Ԃ������Ă���
	orderData_.time_ -= SceneManager::GetInstance().GetDeltaTime();
}

void Order::CreateOrder(void)
{
	int drinkType = 0;
	int sweetsType = 0;

	//�����������߂�
	orderNum_ = GetRand(ORDER_MAX_NUM);
	while(orderNum_ <= 0)
	{
		//������0�ȉ��������������x�������擾����
		orderNum_ = GetRand(ORDER_MAX_NUM);
	}

	//�������e�����߂�
	switch (orderNum_)
	{
	case ORDER_MIN_NUM:
		//���ݕ��̎�ނ�NONE(0)�ȉ��������痐���擾���Ȃ���
		while (drinkType <= 0)	
		{
			drinkType = GetRand(DRINK_MAX_NUM);
		}
		sweetsType = 0;		//���������P�̎��̓X�C�[�c�͗��܂Ȃ��悤�ɂ���
		//��������ݒ�
		SetOrderNum(ORDER_MIN_NUM);
		//���ݕ��̎�ނ�ݒ�
		SetDrink(static_cast<DRINK>(drinkType));	
		//�H�ו��̎�ނ�ݒ�
		SetSweets(static_cast<SWEETS>(sweetsType));	
		//�������Ԃ�ݒ�
		SetOrderTime(ONE_ORDER_TIME);				
		break;

	case ORDER_MAX_NUM:
		//���ݕ��̎�ނ�NONE(0)�ȉ��������痐���擾���Ȃ���
		while (drinkType <= 0)	
		{
			drinkType = GetRand(DRINK_MAX_NUM);
		}

		while (sweetsType <= 0)	//�X�C�[�c�̎�ނ�NONE(0)�ȉ��������痐���擾���Ȃ���
		{
			sweetsType = GetRand(FOOD_MAX_NUM);
		} 
		//��������ݒ�
		SetOrderNum(ORDER_MAX_NUM);
		//���ݕ��̎�ނ�ݒ�
		SetDrink(static_cast<DRINK>(drinkType));
		//�H�ו��̎�ނ�ݒ�
		SetSweets(static_cast<SWEETS>(sweetsType));	
		//�������Ԃ�ݒ�
		SetOrderTime(TWO_ORDER_TIME);				
		break;

	default:
		break;
	}
}