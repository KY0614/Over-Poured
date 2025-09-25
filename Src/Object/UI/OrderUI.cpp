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
	//�摜�̓ǂݍ���
	LoadImages();

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
	//��\���̏ꍇ�͏������Ȃ�
	if (!isActive_)return;

	//�^�C�}�[�̃Q�[�W���C�[�W���O�������Ċ��炩�Ɍ��炵�Ă���
	orderUIData_.displayedRate_ = Easing::Linear(
		gaugeTime_, 1.0f,
		0.0f, orderUIData_.currentRate_);

	//�X�R�A�����݂̃����N�͈̔͂���v�Z����i���100�������ăp�[�Z���g�ɂ���j
	orderUIData_.currentRate_ = orderTimer_ / orderMaxTime_;
	//0��菬�����Ȃ�Ȃ��悤�ɂ���
	if (orderUIData_.currentRate_ <= 0.0f)orderUIData_.currentRate_ = 0.0f;
}

void OrderUI::Draw(void)
{
	//��\���̏ꍇ�͏������Ȃ�
	if (isActive_ != true)return;
	const float imageCenter = 0.5f;
	//�������e�̔w�i��`��
	DrawBillboard3D(pos_, imageCenter, imageCenter, BACK_IMG_SIZE,
		0.0f, orderUIData_.backUIImg_, true);
	//�X�C�[�c�ƃh�����N��UI��`��
	if (orderUIData_.sweetsType_ != Order::SWEETS::NONE)
	{
		//�h�����N�ƃX�C�[�c��UI�̈ʒu����
		VECTOR drinkPos = VAdd(pos_, VGet(-(size_ / 2.0f), 0.0f, 0.0f));
		VECTOR sweetsPos = VAdd(pos_, VGet((size_ / 2.0f),0.0f,0.0f));
		//�h�����N��UI�`��
		DrawBillboard3D(drinkPos, imageCenter, imageCenter, size_,
			0.0f, orderUIData_.drinkUIImg_, true);

		//�X�C�[�c��UI�`��
		DrawBillboard3D(sweetsPos, imageCenter, imageCenter, size_,
			0.0f, orderUIData_.sweetsUIImg_, true);

		if(isOrderCheck_.front())
		{
			//�h�����N�̃`�F�b�N�}�[�N�`��
			DrawBillboard3D(drinkPos, imageCenter, imageCenter, size_,
				0.0f, orderUIData_.checkImg_, true);
		}

		if(isOrderCheck_.back())
		{
			//�X�C�[�c�̃`�F�b�N�}�[�N�`��
			DrawBillboard3D(sweetsPos, imageCenter, imageCenter, size_,
				0.0f, orderUIData_.checkImg_, true);
		}
	}
	else
	{
		//�X�C�[�c���Ȃ��ꍇ�̓h�����N�����`��
		DrawBillboard3D(pos_, imageCenter, imageCenter, size_,
			0.0f, orderUIData_.drinkUIImg_, true);
		//�`�F�b�N�}�[�N�`��
		if (isOrderCheck_.front())
		{
			DrawBillboard3D(pos_, imageCenter, imageCenter, size_,
				0.0f, orderUIData_.checkImg_, true);
		}
	}
	//�I�t�Z�b�g���W�����Z���Ă��烏�[���h���W����X�N���[�����W�ɕϊ�
	const VECTOR offsetPos = VGet(120.0f, -50.0f, 0.0f);
	VECTOR pos = VAdd(pos_, offsetPos);
	VECTOR screenPos = ConvWorldPosToScreenPos(pos);
	//�^�C�}�[�̔w�i��`��
	DrawBillboard3D(pos,
		imageCenter, imageCenter, size_,
		0.0f, orderUIData_.timerBackImg_, true);

	//��ʂ̑傫���ɍ��킹�Ċg�嗦��ς���
	float scale = 
		static_cast<float>(Application::SCREEN_SIZE_Y) / 
		static_cast<float>(Application::SCREEN_MAX_SIZE_Y);
	const float maxRate = 100.0f;
	//�^�C�}�[�̉~�Q�[�W��`��
	DrawCircleGauge(
		(int)screenPos.x,
		(int)screenPos.y,
		orderUIData_.currentRate_ * maxRate,
		orderUIData_.timerImg_,
		0.0f,
		scale,
		false, false
	);
}

void OrderUI::LoadImages(void)
{
	//�����p�̔w�i�i�����o�����ۂ��摜�j�̓ǂݍ���
	orderUIData_.backUIImg_ = ResourceManager::GetInstance().Load(
		ResourceManager::SRC::UI_ORDER_BACK).handleId_;
	const int minOrderSize = 1;
	const int maxOrderSize = 2;
	//�h�����N�pUI
	if (orderUIData_.drinkType_ == Order::DRINK::HOT)
	{
		//�z�b�g�p�摜�ǂݍ���
		orderUIData_.drinkUIImg_ = ResourceManager::GetInstance().Load(
			ResourceManager::SRC::UI_HOT).handleId_;
	}
	else if (orderUIData_.drinkType_ == Order::DRINK::ICE)
	{
		//�A�C�X�p�摜�ǂݍ���
		orderUIData_.drinkUIImg_ = ResourceManager::GetInstance().Load(
			ResourceManager::SRC::UI_ICE).handleId_;
	}
	//�X�C�[�c�pUI
	if (orderUIData_.sweetsType_ == Order::SWEETS::CHOCO)
	{
		//�`���R�p�摜�ǂݍ���
		orderUIData_.sweetsUIImg_ = ResourceManager::GetInstance().Load(
			ResourceManager::SRC::UI_CHOCO).handleId_;
		isOrderCheck_.resize(maxOrderSize);
	}
	else if (orderUIData_.sweetsType_ == Order::SWEETS::STRAWBERRY)
	{
		//�x���[�p�摜�ǂݍ���
		orderUIData_.sweetsUIImg_ = ResourceManager::GetInstance().Load(
			ResourceManager::SRC::UI_BERRY).handleId_;
		isOrderCheck_.resize(maxOrderSize);
	}
	else
	{
		//�X�C�[�c�����̏ꍇ��-1
		orderUIData_.sweetsUIImg_ = -1;
		isOrderCheck_.resize(minOrderSize);
	}

	//�^�C�}�[�w�i�摜
	orderUIData_.timerBackImg_ = ResourceManager::GetInstance().Load(
		ResourceManager::SRC::UI_CIRCLESHADOW).handleId_;
	//�^�C�}�[�摜
	orderUIData_.timerImg_ = ResourceManager::GetInstance().Load(
		ResourceManager::SRC::UI_CIRCLE).handleId_;
	//�`�F�b�N�}�[�N�摜
	orderUIData_.checkImg_ = ResourceManager::GetInstance().Load(
		ResourceManager::SRC::UI_CHECK).handleId_;
}