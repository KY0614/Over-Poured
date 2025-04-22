//#pragma once
#include <vector>
#include <memory>
#include "./CustomerBase.h"


class CustomerManager
{
public:

	//お客の最大人数
	static constexpr int MAX_CUSTOMER_SIZE = 10;

	//生成する上限
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
	// 静的インスタンス
	static CustomerManager* instance_;
	
	//とりあえず一人
	std::vector<std::shared_ptr<CustomerBase>> customers_;
};

