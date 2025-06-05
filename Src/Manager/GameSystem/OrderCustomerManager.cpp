#include <vector>
#include <algorithm>
#include "../../Utility/AsoUtility.h"
#include "../../Common/DebugDrawFormat.h"
#include "../../Manager/Generic/Camera.h"
#include "../../Object/Customer/CustomerManager.h"
#include "../../Object/Order/OrderManager.h"
#include "../../Object/Stage/StageObject.h"
#include "../Generic/SceneManager.h"
#include "OrderCustomerManager.h"

OrderCustomerManager::OrderCustomerManager(void)
{
	orderMng_ = nullptr;
	customerMng_ = nullptr;
	isServe_ = false;
}

OrderCustomerManager::~OrderCustomerManager(void)
{
}

void OrderCustomerManager::Init(void)
{
	//����
	orderMng_ = std::make_unique<OrderManager>();
	orderMng_->Init();

	//�q
	customerMng_ = std::make_shared<CustomerManager>();

	//��������Ă��钍�����e���Q�Ƃ��Ď�ނ����߂Đ���
	CreateCustomersByOrders();
	customerMng_->Init();
	//customerMng_->InitCustomersPos();
	isServe_ = false;
}

void OrderCustomerManager::Update(void)
{
	customerMng_->Update();

	//��������J�E���^�[�܂œ������p
	if (!customerMng_->CheckFirstCustomerCol())
	{
		customerMng_->IsMove();
		if (customerMng_->CheckFirstCustomerCol())customerMng_->IsNotMove();
	}
	
	//�����̐������Ԃ��߂�����ǉ��������s��
	if (orderMng_->IsFirstOrderTimeOut() || isServe_)
	{
		if(!isServe_)customerMng_->SetCustomerReacton(0);
		//���q���ړ�������
		customerMng_->IsMove();

		//���q�̈ړ����I�������폜�Ɛ������s��
		if (customerMng_->CheckSecondCustomerCol())
		{
			//�����Ƃ��q��ǉ�����
			ClearOrderAndCustomer();

			//�ǉ�����
			AddOrdersAndCustomers();
			isServe_ = false;
		}
	}
	else if(customerMng_->CheckFirstCustomerCol())
	{
		//�������Ԃ��߂��ĂȂ�������擪�̃I�[�_�[�����X�V
		orderMng_->FirstOrderUpdate();
	}
}

void OrderCustomerManager::Draw(void)
{
	orderMng_->Draw();

	customerMng_->Draw();

#ifdef _DEBUG

	DebugDraw();

#endif // _DEBUG

}

void OrderCustomerManager::CreateCustomersByOrders(void)
{
	std::vector<Order::DRINK> drinks = orderMng_->GetAllOrderDrink();
	for (Order::DRINK drink : drinks)
	{
		customerMng_->CreateSingleCustomer(drink);
	}
}

void OrderCustomerManager::AddOrdersAndCustomers(void)
{
	//�����Ƃ��q��ǉ������A�ʒu����
	orderMng_->AddOrder();
	AddCustomerByOrder();
}

void OrderCustomerManager::ClearOrderAndCustomer(void)
{
	//�擪�̒����Ƃ��q���폜
	orderMng_->ClearFirstOrder();
	customerMng_->ClearFirstCustomers();
}

void OrderCustomerManager::AddCustomerByOrder(void)
{
	customerMng_->CreateSingleCustomer(orderMng_->GetLastOrderDrink());
	customerMng_->SetLastCustomerPos();
}

int OrderCustomerManager::CheckServeAndOrder(Order::OrderData serve)
{
	Order::DRINK serveDrink = serve.drink_;
	Order::SWEETS serveSweets = serve.sweets_;
	bool serveDrinkLid = serve.lid_;
	int score = 0;

	if(orderMng_->GetFirstOrder().num_ == 1)
	{
		//���ݕ��ƒ񋟕i���r���ĉ��_
		if (serveDrink == orderMng_->GetFirstOrder().drink_)
		{
			score += 50;
		}
		else
		{
			score -= 50;
		}

		//���ݕ��̊W������Ă�������_
		if (serveDrinkLid)
		{
			score += 20;
		}

		//���������P�̂Ƃ��̓T�u�I�[�_�[�͂Ȃ��̂�-50�_
		if (serveSweets != orderMng_->GetFirstOrder().sweets_)
		{
			score -= 50;
		}

		//�I�[�_�[�̎c�萧�����Ԃɂ����_
		if (orderMng_->GetFirstOrder().time_ > 10.0f)
		{
			score += 50;
		}
		else if (orderMng_->GetFirstOrder().time_ > 3.0f)
		{
			score += 10;
		}	
	}
	else if (orderMng_->GetFirstOrder().num_ == 2)
	{
		//���ݕ��ƒ񋟕i���r���ĉ��_
		if (serveDrink == orderMng_->GetFirstOrder().drink_)
		{
			score += 50;
		}
		else
		{
			score -= 50;
		}

		//���ݕ��̊W������Ă�������_
		if (serveDrinkLid)
		{
			score += 20;
		}

		//�T�u�I�[�_�[�̃X�C�[�c�Ƃ̔�r���ĉ��_
		if (serveSweets == orderMng_->GetFirstOrder().sweets_)
		{
			score += 30;
		}
		else
		{
			score -= 20;
		}

		//�I�[�_�[�̎c�萧�����Ԃɂ����_
		if (orderMng_->GetFirstOrder().time_ > 12.0f)
		{
			score += 40;
		}
		else if (orderMng_->GetFirstOrder().time_ > 6.0f)
		{
			score += 20;
		}		
	}
	customerMng_->SetCustomerReacton(score);
	return score;
}

int OrderCustomerManager::CheckServeAndOrder(StageObject& obj)
{
	Order::DRINK serveDrink = obj.GetDrinkType();
	Order::SWEETS serveSweets = obj.GetSweetsType();
	int score = 0;

	if (orderMng_->GetFirstOrder().num_ == 1)
	{
		//���ݕ��ƒ񋟕i���r���ĉ��_
		if (serveDrink == orderMng_->GetFirstOrder().drink_)
		{
			score += 50;
		}
		else
		{
			score -= 50;
		}

		//���������P�̂Ƃ��̓T�u�I�[�_�[�͂Ȃ��̂�-50�_
		if (serveSweets != orderMng_->GetFirstOrder().sweets_)
		{
			score -= 50;
		}

		//�I�[�_�[�̎c�萧�����Ԃɂ����_
		if (orderMng_->GetFirstOrder().time_ > 7.0f)
		{
			score += 50;
		}
		else if (orderMng_->GetFirstOrder().time_ > 3.0f)
		{
			score += 10;
		}
	}
	else
	{
		if (serveDrink == orderMng_->GetFirstOrder().drink_)
		{
			score += 30;
		}

		if (serveSweets == orderMng_->GetFirstOrder().sweets_)
		{
			score += 30;
		}

		if (orderMng_->GetFirstOrder().time_ > 12.0f)
		{
			score += 40;
		}
		else if (orderMng_->GetFirstOrder().time_ > 6.0f)
		{
			score += 20;
		}
	}
	customerMng_->SetCustomerReacton(score);
	return score;
}

bool OrderCustomerManager::GetIsMoving(void)
{
	return customerMng_->GetIsMove();
}

int OrderCustomerManager::GetCustomerNum(void) const
{
	return customerMng_->GetCustomerNum();
}

void OrderCustomerManager::DebugDraw(void)
{
	SetFontSize(24);
	VECTOR pos = customerMng_->GetFirstPos();
	pos.y += 180.0f;
	pos.x += 30.0f;
	VECTOR screenPos = ConvWorldPosToScreenPos(pos);

	VECTOR pos2 = customerMng_->GetFirstPos();
	pos2.y += 150.0f;
	pos2.x += 30.0f;
	VECTOR screenPos2 = ConvWorldPosToScreenPos(pos2);

	// �ϊ�����
	DrawFormatString(static_cast<int>(screenPos.x), static_cast<int>(screenPos.y), GetColor(255, 255, 255),
		L"���� : %d,%d",
		orderMng_->GetFirstOrder().drink_, orderMng_->GetFirstOrder().sweets_);	// �ϊ�����
	DrawFormatString(static_cast<int>(screenPos2.x), static_cast<int>(screenPos2.y), GetColor(255, 255, 255),
		L"������������ : % 2.f",
		orderMng_->GetFirstOrder().time_);

	SetFontSize(16);

	auto orders = orderMng_->GetFirstOrder();

	//�����ɍ��킹�Ďl�p�̐F��ς���
	//int startX = DebugDrawFormat::GetFormatSize(L"���� : %d,%d", orders.drink_, orders.sweets_);
	//startX = startX * 1.5;//�t�H���g�T�C�Y��1.5�{�Ȃ̂�
	//int endX = startX + scale;
	//int startY = 30;
	//int endY = startY + scale;
	int drinkCol = GetColor(0, 0, 0);

	//�\���x�[�X�ʒu�iY�͂�����Ə�ɃI�t�Z�b�g�j
	int baseX = static_cast<int>(screenPos.x);
	int baseY = static_cast<int>(screenPos.y) - 80; // ������Y�ƍ��킹��

	// �l�p�`�̕`��ʒu������i�����̉��ɕ��ׂ�j
	int startX = baseX;
	int startY = baseY + 40; // �����̉��ɏ����󂯂�

	int scale = 25;//�傫��
	int gap = 10; // �����ƃ{�b�N�X�̌���

	//���ݕ��̐F����
	if (orders.drink_ == Order::DRINK::HOT)
	{
		drinkCol = GetColor(255, 0, 0);
	}
	else
	{
		drinkCol = GetColor(0, 255, 255);
	}
	//���ݕ��p
	DrawBox(startX, startY, startX + scale, startY + scale, drinkCol, true);

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
	}

	//�H�ו��p
	DrawBox(startX + scale + gap, startY, startX + scale * 2 + gap, startY + scale, foodCol, true);

#ifdef _DEBUG

	int line = 1;	//�s
	int lineHeight = 30;	//�s

	SetFontSize(24);
	DebugDrawFormat::FormatString(L"���� : %d,%d", orderMng_->GetFirstOrder().drink_, orderMng_->GetFirstOrder().sweets_, line, lineHeight);
	DebugDrawFormat::FormatString(L"������������ : %2.f", orderMng_->GetFirstOrder().time_, line, lineHeight);
	DebugDrawFormat::FormatString(L"%d��", orderMng_->GetCount(), line, lineHeight);
	DebugDrawFormat::FormatString(L"%d�l", customerMng_->GetCustomerNum(), line, lineHeight);
	DebugDrawFormat::FormatString(L"isServe : %d", isServe_, line, lineHeight);
	SetFontSize(16);

#endif // _DEBUG
}
