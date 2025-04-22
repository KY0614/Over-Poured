//#pragma once
#include <vector>
#include <memory>
#include "./CustomerBase.h"


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

	void CreateCustomer(std::shared_ptr<CustomerBase> customer);

	void ClearCustomers(void);

	void CollisionCounter(void);

private:
	// �ÓI�C���X�^���X
	static CustomerManager* instance_;
	
	//�Ƃ肠������l
	std::vector<std::shared_ptr<CustomerBase>> customers_;
};

