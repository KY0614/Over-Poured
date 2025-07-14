#include <vector>
#include <algorithm>
#include "../../Utility/AsoUtility.h"
#include "../../Common/DebugDrawFormat.h"
#include "../../Manager/Generic/Camera.h"
#include "../../Object/Customer/CustomerManager.h"
#include "../../Object/Order/OrderManager.h"
#include "../../Object/Stage/StageObject.h"
#include "../Generic/SceneManager.h"
#include "../../Object/UI/OrderUI.h"
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

	//for (int i = 0; i < orderMng_->GetAllOrderDrink().size(); ++i)
	//{
	//	std::vector<Order::DRINK> drink;
	//	std::vector<Order::SWEETS> sweets;
	//	drink.resize(orderMng_->GetAllOrderDrink().size());
	//	sweets.resize(orderMng_->GetAllOrderSweet().size());
	//	drink = orderMng_->GetAllOrderDrink();
	//	sweets = orderMng_->GetAllOrderSweet();
	//	orderUI_.emplace_back(std::make_unique<OrderUI>(drink[i], sweets[i]));
	//	VECTOR pos = VAdd(
	//		customerMng_->GetPos(i),
	//		VGet(ORDER_UI_OFFSET_X, ORDER_UI_OFFSET_Y, 0.0f));
	//	orderUI_[i]->Init();
	//	orderUI_[i]->SetPos(pos);
	//}

	isServe_ = false;
}

void OrderCustomerManager::Update(void)
{
	customerMng_->Update(orderMng_->GetFirstOrder().time_);
	//for (int i = 0; i < orderMng_->GetAllOrderDrink().size(); ++i)
	//{
	//	VECTOR pos = VAdd(customerMng_->GetPos(i),
	//		VGet(ORDER_UI_OFFSET_X, ORDER_UI_OFFSET_Y,0.0f));
	//	orderUI_[i]->SetPos(pos);
	//}
	////�������璍����\��
	//if (customerMng_->GetFirstState() == CustomerBase::STATE::IDLE)
	//{
	//	orderUI_.front()->SetActive(true);
	//}
	//��������J�E���^�[�܂œ������p
	if (!customerMng_->CheckFirstCustomerCol())
	{
		customerMng_->IsMove();
		//�������瓮���Ȃ��悤��&������\��
		if (customerMng_->CheckFirstCustomerCol())
		{
			customerMng_->IsNotMove();
		}
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
			////UI���ǉ�����
			//orderUI_.emplace_back(std::make_unique<OrderUI>(
			//	orderMng_->GetLastOrderDrink(), orderMng_->GetLastOrderSweets()));
			//VECTOR pos = VAdd(
			//	customerMng_->GetLastPos(),
			//	VGet(ORDER_UI_OFFSET_X, ORDER_UI_OFFSET_Y, 0.0f));
			//orderUI_.back()->Init();
			//orderUI_.back()->SetPos(pos);
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

	//for (const auto& ui : orderUI_)
	//{
	//	ui->Draw();
	//}

#ifdef _DEBUG

	//DebugDraw();

#endif // _DEBUG

}

void OrderCustomerManager::CreateCustomersByOrders(void)
{
	std::vector<Order::DRINK> drinks = orderMng_->GetAllOrderDrink();
	std::vector<Order::SWEETS> sweets = orderMng_->GetAllOrderSweet();
	std::vector<Order::OrderData> data = orderMng_->GetAllOrder();
	for(int i = 0; i < data.size();++i)
	{
		customerMng_->CreateSingleCustomer(data[i]);
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
	customerMng_->CreateSingleCustomer(orderMng_->GetLastOrderData());
	customerMng_->SetLastCustomerPos();
}

int OrderCustomerManager::CheckServeAndOrder(const Order::OrderData serve)
{
	Order::DRINK serveDrink = serve.drink_;
	Order::SWEETS serveSweets = serve.sweets_;
	bool serveDrinkLid = serve.lid_;
	const auto& order = orderMng_->GetFirstOrder();
	int score = 0;

	// �h�����N����
	if (order.drink_ != Order::DRINK::NONE) 
	{
		if (serve.drink_ == order.drink_) 
		{
			score += 50;
			if (serve.lid_) score += 20;
		}
		else 
		{
			score -= 50;
		}
	}
	// �X�C�[�c����
	if (order.sweets_ != Order::SWEETS::NONE) 
	{
		if (serve.sweets_ == order.sweets_) 
		{
			score += 30;
		}
		else 
		{
			score -= 20;
		}
	}
	// ���ԃ{�[�i�X
	if (order.num_ == 1) 
	{
		if (order.time_ > 6.0f) score += 50;
		else if (order.time_ > 3.0f) score += 10;
	}
	else if (order.num_ == 2) 
	{
		if (order.time_ > 12.0f) score += 40;
		else if (order.time_ > 6.0f) score += 20;
	}

	//if(orderMng_->GetFirstOrder().num_ == 1)
	//{
	//	//���ݕ��ƒ񋟕i���r���ĉ��_
	//	if (serveDrink == orderMng_->GetFirstOrder().drink_)
	//	{
	//		score += 50;
	//	}
	//	else
	//	{
	//		score -= 50;
	//	}

	//	//���ݕ��̊W������Ă�������_
	//	if (serveDrinkLid)
	//	{
	//		score += 20;
	//	}

	//	//���������P�̂Ƃ��̓T�u�I�[�_�[�͂Ȃ��̂�-50�_
	//	if (serveSweets != orderMng_->GetFirstOrder().sweets_)
	//	{
	//		score -= 50;
	//	}

	//	//�I�[�_�[�̎c�萧�����Ԃɂ����_
	//	if (orderMng_->GetFirstOrder().time_ > 6.0f)
	//	{
	//		score += 50;
	//	}
	//	else if (orderMng_->GetFirstOrder().time_ > 3.0f)
	//	{
	//		score += 10;
	//	}	
	//}
	//else if (orderMng_->GetFirstOrder().num_ == 2)
	//{
	//	//���ݕ��ƒ񋟕i���r���ĉ��_
	//	if (serveDrink == orderMng_->GetFirstOrder().drink_)
	//	{
	//		score += 50;
	//	}
	//	else
	//	{
	//		score -= 50;
	//	}

	//	//���ݕ��̊W������Ă�������_
	//	if (serveDrinkLid)
	//	{
	//		score += 20;
	//	}

	//	//�T�u�I�[�_�[�̃X�C�[�c�Ƃ̔�r���ĉ��_
	//	if (serveSweets == orderMng_->GetFirstOrder().sweets_)
	//	{
	//		score += 30;
	//	}
	//	else
	//	{
	//		score -= 20;
	//	}

	//	//�I�[�_�[�̎c�萧�����Ԃɂ����_
	//	if (orderMng_->GetFirstOrder().time_ > 12.0f)
	//	{
	//		score += 40;
	//	}
	//	else if (orderMng_->GetFirstOrder().time_ > 6.0f)
	//	{
	//		score += 20;
	//	}		
	//}
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

Order::OrderData OrderCustomerManager::GetOrderData(void) const
{
	return orderMng_->GetFirstOrder();
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

	//���q�̈ʒu�ɒ������o��
	DrawFormatString(static_cast<int>(screenPos.x), static_cast<int>(screenPos.y), GetColor(255, 255, 255),
		L"���� : %d,%d",
		orderMng_->GetFirstOrder().drink_, orderMng_->GetFirstOrder().sweets_);
	DrawFormatString(static_cast<int>(screenPos2.x), static_cast<int>(screenPos2.y), GetColor(255, 255, 255),
		L"������������ : % 2.f",
		orderMng_->GetFirstOrder().time_);

	SetFontSize(16);

	auto orders = orderMng_->GetFirstOrder();

	//�����ɍ��킹�Ďl�p�̐F��ς���
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

	//int line = 1;	//�s
	//int lineHeight = 30;	//�s

	//SetFontSize(24);
	//DebugDrawFormat::FormatString(L"���� : %d,%d", orderMng_->GetFirstOrder().drink_, orderMng_->GetFirstOrder().sweets_, line, lineHeight);
	//DebugDrawFormat::FormatString(L"������������ : %2.f", orderMng_->GetFirstOrder().time_, line, lineHeight);
	//DebugDrawFormat::FormatString(L"%d��", orderMng_->GetCount(), line, lineHeight);
	//DebugDrawFormat::FormatString(L"%d�l", customerMng_->GetCustomerNum(), line, lineHeight);
	//DebugDrawFormat::FormatString(L"isServe : %d", isServe_, line, lineHeight);
	//SetFontSize(16);

#endif // _DEBUG
}
