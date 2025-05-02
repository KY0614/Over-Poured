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
	/// �擪�̂��q�̈ʒu���擾
	/// </summary>
	/// <param name="pos"></param>
	void SetFirstCustomerPos(VECTOR pos) { prePos_ = pos; }

	VECTOR SetLastCustomerPos(void);

	void IsMoveFirstCustomer(void) { customers_.front()->IsMove(); }
	void SetIsMoveFCustomer(bool isMove) { customers_.front()->SetIsMove(isMove); }

	bool GetIsMoving(void) { return customers_.front()->GetIsMove(); }

	int GetCustomerNum(void) { return customers_.size(); }
	VECTOR GetFirstPos(void) { return customers_.front()->GetPos(); }
	VECTOR GetSecondPos(void) { return customers_[1]->GetPos(); }
	VECTOR GetLastPos(void) { return customers_.back()->GetPos(); }

	bool GetCustomerMove(void) { return customers_.front()->GetIsMove(); }

	bool CheckFirstCustomerCol(void);

private:
	// �ÓI�C���X�^���X
	static CustomerManager* instance_;
	
	//�Ƃ肠������l
	std::vector<std::shared_ptr<CustomerBase>> customers_;

	VECTOR prePos_;
};

