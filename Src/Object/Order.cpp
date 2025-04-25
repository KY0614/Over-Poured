#include <DxLib.h>
#include "../Libs/ImGui/imgui.h"
#include "../Common/DebugDrawFormat.h"
#include "../Manager/Generic/SceneManager.h"
#include "../Manager/GameSystem/OrderManager.h"
#include "../Manager/Generic/InputManager.h"
#include "Order.h"

Order::Order(void)
{

}

Order::~Order(void)
{
}

void Order::Init(void)
{

}

void Order::Update(void)
{
	//�������Ԃ���o�ߎ��Ԃ������Ă���
	orderData_.time_ -= SceneManager::GetInstance().GetDeltaTime();

#ifdef _DEBUG

	//ImGui�̑�����s��
	//UpdateDebugImGui();

#endif // _DEBUG

}

void Order::Draw(void)
{

}

void Order::CreateOrder(void)
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
		SetOrderNum(1);
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
		SetOrderNum(2);
		SetDrink(static_cast<DRINK>(drinkType));	//���ݕ��̎�ނ�ݒ�
		SetSweets(static_cast<SWEETS>(sweetsType));	//�H�ו��̎�ނ�ݒ�

		SetOrderTime(TWO_ORDER_TIME);				//�������Ԃ�ݒ�
		break;

	default:
		break;
	}
}

//void Order::UpdateImgGUI(void)
//{
//	//�E�B���h�E�^�C�g��&�J�n����
//	ImGui::Begin("Box");
//	//�傫��
//	ImGui::Text("scale");
//	ImGui::InputFloat("SclX", &transform_.scl.x);
//	ImGui::InputFloat("SclY", &transform_.scl.y);
//	ImGui::InputFloat("SclZ", &transform_.scl.z);
//	//�p�x
//	VECTOR rotDeg = VECTOR();
//	rotDeg.x = AsoUtility::Rad2DegF(transform_.rot.x);
//	rotDeg.y = AsoUtility::Rad2DegF(transform_.rot.y);
//	rotDeg.z = AsoUtility::Rad2DegF(transform_.rot.z);
//	ImGui::Text("angle(deg)");
//	ImGui::SliderFloat("RotX", &rotDeg.x, 0.0f, 360.0f);
//	ImGui::SliderFloat("RotY", &rotDeg.y, 0.0f, 360.0f);
//	ImGui::SliderFloat("RotZ", &rotDeg.z, 0.0f, 360.0f);
//	transform_.rot.x = AsoUtility::Deg2RadF(rotDeg.x);
//	transform_.rot.y = AsoUtility::Deg2RadF(rotDeg.y);
//	transform_.rot.z = AsoUtility::Deg2RadF(rotDeg.z);
//	//�ʒu
//	ImGui::Text("position");
//	//�\���̂̐擪�|�C���^��n���Axyz�ƘA�������������z�u�փA�N�Z�X
//	ImGui::InputFloat3("Pos", &transform_.pos.x);
//	//�I������
//	ImGui::End();
//}
