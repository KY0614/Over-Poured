//#pragma once
#include <vector>
#include <memory>
#include "../../Manager/GameSystem/OrderManager.h"

class CustomerBase;

class CustomerManager
{
public:
	//�C���X�^���X�̐���
	static void CreateInstance(void);

	//�C���X�^���X�̎擾
	static CustomerManager& GetInstance(void);

	void Init(void);
	void Update(void);
	void Draw(void);

	//���\�[�X�̔j��
	void Destroy(void);

	void CreateCustomer(OrderManager& order);

	void AddCustomers(void);

private:
	// �ÓI�C���X�^���X
	static CustomerManager* instance_;
	
	std::vector<std::shared_ptr<CustomerBase>> customers_;
};

