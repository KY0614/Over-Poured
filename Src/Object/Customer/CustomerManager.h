#pragma once
#include <vector>
#include <memory>
#include "./CustomerBase.h"
#include "../Order.h"


class CustomerManager
{
public:

	//���q�̍ő�l��
	static constexpr int MAX_CUSTOMER_SIZE = 10;

	//����������
	static constexpr int MAX_CREATE_SIZE = 6;

	//���q���m�̊Ԋu
	static constexpr float CUSTOMERS_SPACE = 90.0f;

	CustomerManager(void);
	~CustomerManager(void);

	void Init(void);
	void Update(void);
	void Draw(void);

	//���q����
	
	/// <summary>
	/// �ŏ���5�l�̍��W��������
	/// </summary>
	void InitCustomersPos(void);	

	/// <summary>
	/// ���q����l��������
	/// </summary>
	/// <param name="order">��������^�C�v</param>
	void CreateSingleCustomer(Order::DRINK order);

	/// <summary>
	/// ���q��ǉ�����������Ɉʒu��������p
	/// </summary>
	void MoveCustomerPos(void);

	/// <summary>
	/// �z��̐擪�̂��q�����폜
	/// </summary>
	void ClearFirstCustomers(void);

	/// <summary>
	/// �q�𓮂���
	/// </summary>
	void IsMove(void) { isMove_ = true; }

	/// <summary>
	/// �q���~�߂�
	/// </summary>
	/// <param name=""></param>
	void IsNotMove(void) { isMove_ = false; }

	/// <summary>
	/// �q�������Ă��邩�擾����
	/// </summary>
	/// <param name=""></param>
	/// <returns>isMove�̒l��Ԃ�</returns>
	bool GetIsMove(void) { return isMove_; }

	/// <summary>
	/// �X�R�A�ɂ�邨�q�̔����̐ݒ�
	/// </summary>
	/// <param name="score">�X�R�A</param>
	void SetCustomerReacton(int score);

	CustomerBase::STATE GetFrontState(void) { return customers_.front()->GetState(); }

	VECTOR SetLastCustomerPos(void);

	int GetCustomerNum(void) { return customers_.size(); }
	VECTOR GetFirstPos(void) { return customers_.front()->GetPos(); }
	VECTOR GetSecondPos(void) { return customers_[1]->GetPos(); }
	VECTOR GetLastPos(void) { return customers_.back()->GetPos(); }


	bool CheckFirstCustomerCol(void);
	bool CheckSecondCustomerCol(void);

private:
	// �ÓI�C���X�^���X
	static CustomerManager* instance_;
	
	//�Ƃ肠������l
	std::vector<std::shared_ptr<CustomerBase>> customers_;

	bool isMove_;
};

