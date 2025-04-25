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
	static constexpr int MAX_CREATE_SIZE = 5;

	CustomerManager(void);
	~CustomerManager(void);

	void Init(void);
	void Update(void);
	void Draw(void);

	//���q����
	
	/// <summary>
	/// customers_���󂾂�����T�܂Ő�������
	/// </summary>
	/// <param name=""></param>
	void InitFiveCustomer(Order::DRINK drink);		//����������5�l��������p

	void CreateCustomer(Order::DRINK order);	//��l������

	void ClearFirstCustomers(void);

	void CollisionCounter(void);

	void Move2PrePos(void);

	void SetFirstPos(VECTOR pos) { prePos_ = pos; }

	int GetCustomerNum(void) { return customers_.size(); }
	VECTOR GetFirstPos(void) { return customers_.front()->GetPos(); }
	VECTOR GetLastPos(void) { return customers_.back()->GetPos(); }

private:
	// �ÓI�C���X�^���X
	static CustomerManager* instance_;
	
	//�Ƃ肠������l
	std::vector<std::shared_ptr<CustomerBase>> customers_;

	VECTOR prePos_;
};

