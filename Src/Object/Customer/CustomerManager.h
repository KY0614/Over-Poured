//#pragma once
#include <vector>
#include <memory>
#include "../../Manager/GameSystem/OrderManager.h"

class CustomerBase;

class CustomerManager
{
public:
	static constexpr int MAX_CUSTOMER_SIZE = 6;


	CustomerManager(void);
	~CustomerManager(void);

	void Init(void);
	void Update(void);
	void Draw(void);

	void CreateCustomer(OrderManager& order);

	void AddCustomers(void);

	void CollisionCounter(void);

private:
	// 静的インスタンス
	static CustomerManager* instance_;
	
	std::shared_ptr<CustomerBase> customers_[MAX_CUSTOMER_SIZE];
};

