#include "../../Application.h"
#include "../Common/Easing.h"
#include "../../Manager/Generic/ResourceManager.h"
#include "../../Manager/Generic/SceneManager.h"
#include "OrderUI.h"

OrderUI::OrderUI(Order::DRINK drink, Order::SWEETS sweets, float maxTime)
{
	orderUIData_.drinkType_ = drink;
	orderUIData_.sweetsType_ = sweets;
	orderMaxTime_ = maxTime;

	orderUIData_.drinkUIImg_ = -1;
	orderUIData_.sweetsUIImg_ = -1;
	orderUIData_.backUIImg_ = -1;
	orderUIData_.timerBackImg_ = -1;
	orderUIData_.timerImg_ = -1;
	orderUIData_.checkImg_ = -1;
	isOrderCheck_.clear();
	orderTimer_ = 0.0f;
	orderUIData_.currentRate_ = 0.0f;
	orderUIData_.displayedRate_ = 0.0f;
	gaugeTime_ = 0.0f;
	isActive_ = false;
	alpha_ = 1.0f;
	size_ = 0.0f;
}

void OrderUI::Init(void)
{
	orderUIData_.backUIImg_ = ResourceManager::GetInstance().Load(
		ResourceManager::SRC::UI_ORDER_BACK).handleId_;

	//�h�����N�pUI
	if (orderUIData_.drinkType_ == Order::DRINK::HOT)
	{
		orderUIData_.drinkUIImg_ = ResourceManager::GetInstance().Load(
			ResourceManager::SRC::UI_HOT).handleId_;
	}
	else if (orderUIData_.drinkType_ == Order::DRINK::ICE)
	{
		orderUIData_.drinkUIImg_ = ResourceManager::GetInstance().Load(
			ResourceManager::SRC::UI_ICE).handleId_;
	}
	//�X�C�[�c�pUI
	if (orderUIData_.sweetsType_ == Order::SWEETS::CHOCO)
	{
		orderUIData_.sweetsUIImg_ = ResourceManager::GetInstance().Load(
			ResourceManager::SRC::UI_CHOCO).handleId_;
		isOrderCheck_.resize(2);
	}
	else if (orderUIData_.sweetsType_ == Order::SWEETS::STRAWBERRY)
	{
		orderUIData_.sweetsUIImg_ = ResourceManager::GetInstance().Load(
			ResourceManager::SRC::UI_BERRY).handleId_;
		isOrderCheck_.resize(2);
	}
	else
	{
		orderUIData_.sweetsUIImg_ = -1;
		isOrderCheck_.resize(1);
	}
	//�^�C�}�[�w�i�摜
	orderUIData_.timerBackImg_ = ResourceManager::GetInstance().Load(
		ResourceManager::SRC::UI_CIRCLESHADOW).handleId_;
	//�^�C�}�[�摜
	orderUIData_.timerImg_ = ResourceManager::GetInstance().Load(
		ResourceManager::SRC::UI_CIRCLE).handleId_;
	//�`�F�b�N�摜
	orderUIData_.checkImg_ = ResourceManager::GetInstance().Load(
		ResourceManager::SRC::UI_CHECK).handleId_;

	//�}�[�N�͍ŏ��͔�\��
	for(auto check : isOrderCheck_)
	{
		check = false;
	}
	//�摜�T�C�Y
	size_ = IMG_SIZE;
}

void OrderUI::Update(void)
{
	if (!isActive_)return;
	orderUIData_.displayedRate_ = Easing::Linear(
		gaugeTime_, 1.0f,
		0.0f, orderUIData_.currentRate_);

	//�X�R�A�����݂̃����N�͈̔͂���v�Z����i���100�������ăp�[�Z���g�ɂ���j
	orderUIData_.currentRate_ = orderTimer_ / orderMaxTime_;
	if (orderUIData_.currentRate_ <= 0.0f)orderUIData_.currentRate_ = 0.0f;
}

void OrderUI::Draw(void)
{
	if (isActive_ != true)return;
	//�������e�̔w�i��`��
	DrawBillboard3D(pos_, 0.5f, 0.5f, BACK_IMG_SIZE,
		0.0f, orderUIData_.backUIImg_, true);
	//�X�C�[�c�ƃh�����N��UI��`��
	if (orderUIData_.sweetsType_ != Order::SWEETS::NONE)
	{
		VECTOR drinkPos = VAdd(pos_, VGet(-(size_ / 2.0f), 0.0f, 0.0f));
		VECTOR sweetsPos = VAdd(pos_, VGet((size_ / 2.0f),0.0f,0.0f));
		DrawBillboard3D(drinkPos, 0.5f, 0.5f, size_,
			0.0f, orderUIData_.drinkUIImg_, true);

		DrawBillboard3D(sweetsPos, 0.5f, 0.5f, size_,
			0.0f, orderUIData_.sweetsUIImg_, true);

		if(isOrderCheck_.front())//�h�����N�̃`�F�b�N�}�[�N�`��
		{
			DrawBillboard3D(drinkPos, 0.5f, 0.5f, size_,
				0.0f, orderUIData_.checkImg_, true);
		}

		if(isOrderCheck_.back())//�X�C�[�c�̃`�F�b�N�}�[�N�`��
		{
			DrawBillboard3D(sweetsPos, 0.5f, 0.5f, size_,
				0.0f, orderUIData_.checkImg_, true);
		}
	}
	else//�X�C�[�c���Ȃ��ꍇ�̓h�����N�����`��
	{
		DrawBillboard3D(pos_, 0.5f, 0.5f, size_,
			0.0f, orderUIData_.drinkUIImg_, true);
		//�`�F�b�N�}�[�N�`��
		if (isOrderCheck_.front())
		{
			DrawBillboard3D(pos_, 0.5f, 0.5f, size_,
				0.0f, orderUIData_.checkImg_, true);
		}
	}

	VECTOR pos = VAdd(pos_, VGet(120.0f, -50.0f, 0.0f));
	VECTOR screenPos = ConvWorldPosToScreenPos(pos);
	//�^�C�}�[�̔w�i��`��
	DrawBillboard3D(pos,
		0.5f, 0.5f, size_,
		0.0f, orderUIData_.timerBackImg_, true);

	//��ʂ̑傫���ɍ��킹�Ċg�嗦��ς���
	float scale = 
		static_cast<float>(Application::SCREEN_SIZE_Y) / 
		static_cast<float>(Application::SCREEN_MAX_SIZE_Y);
	//�^�C�}�[�̉~�Q�[�W��`��
	DrawCircleGauge(
		(int)screenPos.x,
		(int)screenPos.y,
		orderUIData_.currentRate_ * 100.0f,
		orderUIData_.timerImg_,
		0.0f,
		scale,
		false, false
	);
}
