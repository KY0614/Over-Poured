#pragma once
#include <vector>
#include <memory>
#include "./CustomerBase.h"
#include "../Order.h"


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

	//お客生成
	
	/// <summary>
	/// customers_が空だったら５つまで生成する
	/// </summary>
	/// <param name=""></param>
	void InitFiveCustomer(Order::DRINK drink);		//初期化して5人生成する用

	void CreateCustomer(Order::DRINK order);	//一人ずつ生成

	void ClearFirstCustomers(void);

	void CollisionCounter(void);

	void Move2PrePos(void);

	int GetCustomerNum(void) { return customers_.size(); }
	VECTOR GetPos(void) { return customers_[1]->GetPos(); }

private:
	// 静的インスタンス
	static CustomerManager* instance_;
	
	//とりあえず一人
	std::vector<std::shared_ptr<CustomerBase>> customers_;

};

