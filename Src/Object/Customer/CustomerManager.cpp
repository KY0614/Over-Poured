#include "../../Utility/AsoUtility.h"
#include "../../Object/Order/Order.h"
#include "../../Object/Customer/HotCustomer.h"
#include "../../Object/Customer/IceCustomer.h"
#include "../../Object/UI/OrderUI.h"
#include "../../Object/UI/UIManager.h"
#include "CustomerManager.h"

namespace
{
	//����������
	const int MAX_CREATE_SIZE = 6;

	//���q���m�̊Ԋu
	const float CUSTOMERS_SPACE = 90.0f;

	//����UI�̕\���ʒu
	const float ORDER_UI_OFFSET_X = 130.0f;		//X���W�̃I�t�Z�b�g
	const float ORDER_UI_OFFSET_Y = 220.0f;		//Y���W�̃I�t�Z�b�g

	//��]���̊p�x
	const float CUSTOMER_ROTATE_ANGLE = 90.0f;

	const int SCORE_GOOD = 80;	//GOOD�̃X�R�A
	const int SCORE_SOSO = 50;	//SOSO�̃X�R�A
}

CustomerManager::CustomerManager(void)
{
	isCustomersMove_ = false;
	firstCustomerIdx_ = 0;
}

CustomerManager::~CustomerManager(void)
{
	customers_.clear();
}

void CustomerManager::Init(void)
{
	//���q��̏����ʒu��ݒ�
	InitCustomersPos();

	//���q�𓮂����t���O�𗧂Ă�
	isCustomersMove_ = true;
	//�擪�̂��q�̃C���f�b�N�X��������
	firstCustomerIdx_ = 0;
}

void CustomerManager::Update(float orderTime)
{
	//���q�̍X�V����
	for (auto& c : customers_)
	{
		c->Update();
	}
	//����UI�̍X�V����
	for (auto& ui : orderUI_)
	{
		ui->Update();
	}
	//���q�������Ă�����A�S���̈ʒu���X�V�����s�A�j���[�V�������Đ�
	if (isCustomersMove_)
	{
		for (auto& c : customers_)
		{
			c->SetState(CustomerBase::STATE::WALK);
			c->Move();
		}
	}

	//UI���W�����q�̍��W�ƍ��킹��
	for (int i = 0; i < customers_.size(); ++i)
	{
		//UI�̈ʒu�����q�̈ʒu����I�t�Z�b�g�����炷
		VECTOR pos = VAdd(customers_[i]->GetPos(),
			VGet(ORDER_UI_OFFSET_X, ORDER_UI_OFFSET_Y,0.0f));
		orderUI_[i]->SetPos(pos);	//UI�̈ʒu��ݒ�
		orderUI_[i]->SetOrderTimer(orderTime);	//�������Ԃ��X�V
	}

	//�J�E���^�[�O�̋��̔���ɗ�����A��]�����ăJ�E���^�[�̕�������悤�ɂ���
	if (customers_[firstCustomerIdx_]->CollisionCounter())
	{	
		if (customers_[firstCustomerIdx_]->CheckCounterToCustomer())
		{
			//��]�����A�ړ����~�߂�
			customers_[firstCustomerIdx_]->SetGoalRotate(AsoUtility::Deg2RadF(CUSTOMER_ROTATE_ANGLE));
			isCustomersMove_ = false;
			orderUI_[firstCustomerIdx_]->SetActive(true);	//UI���A�N�e�B�u�ɂ���
			for (auto& c : customers_)
			{
				//���s�A�j���[�V�������~�߁AIDLE�A�j���[�V�����ɂ���
				c->SetState(CustomerBase::STATE::IDLE);
			}
		}
	}
}

void CustomerManager::Draw(void)
{
	//���q�̕`�揈��
	for (auto& c : customers_)
	{
		c->Draw();
	}
}

void CustomerManager::CreateSingleCustomer(Order::OrderData data)
{
	//�����pUI�𐶐�
	orderUI_.emplace_back(std::make_unique<OrderUI>(
		data.drink_, data.sweets_,data.time_));

	//�����ɉ��������q�𐶐�
	switch (data.drink_)
	{
	case Order::DRINK::NONE:
		break;

	case Order::DRINK::HOT:
		//HOT�̂��q�𐶐�
		customers_.emplace_back(std::make_unique<HotCustomer>());
		customers_.back()->Init(GetLastCustomerPos());

		{//UI�̈ʒu��ݒ�
			VECTOR pos = VAdd(
				GetLastCustomerPos(),
				VGet(ORDER_UI_OFFSET_X, ORDER_UI_OFFSET_Y, 0.0f));
			orderUI_.back()->Init();
			orderUI_.back()->SetPos(pos);
		}
		break;

	case Order::DRINK::ICE:
		//ICE�̂��q�𐶐�
		customers_.emplace_back(std::move(std::make_unique<IceCustomer>()));
		customers_.back()->Init(GetLastCustomerPos());

		{//UI�̈ʒu��ݒ�
			VECTOR pos = VAdd(
				GetLastCustomerPos(),
				VGet(ORDER_UI_OFFSET_X, ORDER_UI_OFFSET_Y, 0.0f));
			orderUI_.back()->Init();
			orderUI_.back()->SetPos(pos);
		}
		break;

	default:
		break;
	}
	//UI��Manager�ɓo�^
	UIManager::GetInstance().AddOrderUI(orderUI_.back().get());
}

void CustomerManager::ClearFirstCustomers(void)
{
	if (!customers_.empty())
	{
		//�擪�̂��q���\���ɂ���
		customers_[firstCustomerIdx_]->IsNotVisible();
		orderUI_[firstCustomerIdx_]->SetActive(false);
		//�擪�̂��q�̃C���f�b�N�X��i�߂�
		firstCustomerIdx_++;
	}
}

void CustomerManager::SetCustomerReacton(int score)
{
	//�X�R�A�ɉ����ă��A�N�V������ς���
	if (score >= SCORE_GOOD)
	{
		customers_[firstCustomerIdx_]->SetReaction(CustomerBase::REACTION::GOOD);
	}
	else if (score > SCORE_SOSO)
	{
		customers_[firstCustomerIdx_]->SetReaction(CustomerBase::REACTION::SOSO);
	}
	else
	{
		customers_[firstCustomerIdx_]->SetReaction(CustomerBase::REACTION::BAD);
	}
}

VECTOR CustomerManager::GetLastCustomerPos(void) const
{
	//�Ԃ��p�̍��W
	VECTOR retPos;

	//�Ō�̂��q�̈ʒu���擾
	retPos = customers_[firstCustomerIdx_]->GetPos();

	//�Ō�̂��q�̈ʒu����Ԋu�������č��ɂ��炷
	retPos.x -= ((MAX_CREATE_SIZE - 1) * CUSTOMERS_SPACE);

	return retPos;
}

bool CustomerManager::CheckFirstCustomerCol(void)
{
	bool ret = false;
	//�擪�̂��q���J�E���^�[�O�ɂ��邩�ǂ���
	if (customers_[firstCustomerIdx_]->CollisionCounter())
	{
		//�����ꍇ��true��Ԃ�
		ret = true;
	}
	return ret;
}

bool CustomerManager::CheckSecondCustomerCol(void)
{
	bool ret = false;
	//2�Ԗڂ̂��q���J�E���^�[�O�ɂ��邩�ǂ���
	if (customers_[firstCustomerIdx_ + 1]->CollisionCounter())
	{
		//�����ꍇ��true��Ԃ�
		ret = true;
	}
	return ret;
}

void CustomerManager::IsCheckUI(const int index, const bool isActive)
{
	orderUI_[firstCustomerIdx_]->SetCheckUI(index, isActive);
}

void CustomerManager::InitCustomersPos(void)
{
	//���q�̏����ʒu�ݒ�
	VECTOR pos = CustomerBase::CUSTOMER_POS;

	//�S���̈ʒu��x���ō��ɂ��炷
	for (int i = 0; i < MAX_CREATE_SIZE; i++)
	{
		auto& customer = customers_[i];
		//��l�����炷
		pos.x -= CUSTOMERS_SPACE;
		customer->Init(pos);
	}
}
