#include <DxLib.h>
#include "../Libs/ImGui/imgui.h"
#include "../Common/DebugDrawFormat.h"
#include "../Manager/Generic/SceneManager.h"
#include "../Manager/Generic/InputManager.h"
#include "OrderManager.h"
#include "Order.h"

namespace 
{
	//�����֘A
	const int ORDER_MAX_NUM = 2;			//�����̍ő吔
	const int ORDER_MIN_NUM = 1;			//�����̍ŏ���

	const float ONE_ORDER_TIME = 25.0f;	//�����̐�������(1��)
	const float TWO_ORDER_TIME = 33.0f;	//�����̐�������(�Q��)

	//���i�֘A
	const int DRINK_MAX_NUM = 2;			//���ݕ��ނ̍ő吔
	const int FOOD_MAX_NUM = 2;			//�H�ו��ނ̍ő吔
}

Order::Order(void): orderData_()
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
	int orderNum = 0;

	//�����������߂�
	orderNum = GetRand(ORDER_MAX_NUM);
	while(orderNum <= 0)
	{
		//������0�ȉ��������������x�������擾����
		orderNum = GetRand(ORDER_MAX_NUM);
	}

	//��������ݒ�
	SetOrderNum(orderNum);	

	//���ݕ��̎�ނ𗐐��Ŏ擾
	while (drinkType <= 0)	//NONE(0)�ȉ��������痐���擾���Ȃ���
	{
		drinkType = GetRand(DRINK_MAX_NUM);
	}

	//���ݕ��̎�ނ�ݒ�
	SetDrink(static_cast<DRINK>(drinkType));

	//���������Ƃ̐������Ԃ�ݒ�
	if (orderNum > 0)
	{
		//�������Ԃ�ݒ�i��������1�̎��j
		SetOrderTime(ONE_ORDER_TIME);
	}
	else if(orderNum > ORDER_MIN_NUM)
	{
		//�������Ԃ�ݒ�i��������2�̎��j
		SetOrderTime(TWO_ORDER_TIME);
	}

	if (orderNum < ORDER_MAX_NUM)
	{
		//��������1�̎��̓X�C�[�c�͗��܂Ȃ��悤�ɂ���
		return;
	}

	//�X�C�[�c�̎�ނ𗐐��Ŏ擾
	while (sweetsType <= 0)	//�X�C�[�c�̎�ނ�NONE(0)�ȉ��������痐���擾���Ȃ���
	{
		sweetsType = GetRand(FOOD_MAX_NUM);
	}

	//�H�ו��̎�ނ�ݒ�
	SetSweets(static_cast<SWEETS>(sweetsType));
}