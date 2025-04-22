//#pragma once
#include <vector>
#include <memory>
#include "../../Manager/GameSystem/OrderManager.h"

class CustomerBase;

class CustomerManager
{
public:
	//インスタンスの生成
	static void CreateInstance(void);

	//インスタンスの取得
	static CustomerManager& GetInstance(void);

	void Init(void);
	void Update(void);
	void Draw(void);

	//リソースの破棄
	void Destroy(void);

	void CreateCustomer(OrderManager& order);

	void AddCustomers(void);

private:
	// 静的インスタンス
	static CustomerManager* instance_;
	
	std::vector<std::shared_ptr<CustomerBase>> customers_;
};

