//#pragma once
#include <vector>
#include <memory>
#include "./CustomerBase.h"
#include "../Order.h"
#include "../../Manager/GameSystem/OrderManager.h"


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
	void InitCustomer(void);		//�������p
	void CreateCustomer(Order::DRINK order);	//��l������

	void ClearCustomers(void);

	void CollisionCounter(void);

	int GetCustomerNum(void) { return customers_.size(); }
	VECTOR GetPos(void) { return customers_[1]->GetPos(); }

private:
	// �ÓI�C���X�^���X
	static CustomerManager* instance_;
	
	//�Ƃ肠������l
	std::vector<std::shared_ptr<CustomerBase>> customers_;

	Order::DRINK customerType_[MAX_CREATE_SIZE];
};

