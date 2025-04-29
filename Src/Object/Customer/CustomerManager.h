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
	static constexpr int MAX_CREATE_SIZE = 6;

	//お客同士の間隔
	static constexpr float CUSTOMERS_SPACE = 90.0f;

	CustomerManager(void);
	~CustomerManager(void);

	void Init(void);
	void Update(void);
	void Draw(void);

	//お客生成
	
	/// <summary>
	/// 最初の5人の座標を初期化
	/// </summary>
	void InitCustomersPos(void);	

	/// <summary>
	/// お客を一人生成する
	/// </summary>
	/// <param name="order">生成するタイプ</param>
	void CreateSingleCustomer(Order::DRINK order);

	/// <summary>
	/// お客を追加生成した後に位置調整する用
	/// </summary>
	void MoveCustomerPos(void);

	/// <summary>
	/// 配列の先頭のお客情報を削除
	/// </summary>
	void ClearFirstCustomers(void);

	/// <summary>
	/// 先頭のお客の位置を取得
	/// </summary>
	/// <param name="pos"></param>
	void SetFirstCustomerPos(VECTOR pos) { prePos_ = pos; }

	void SetLastCustomerPos(void);

	int GetCustomerNum(void) { return customers_.size(); }
	VECTOR GetFirstPos(void) { return customers_.front()->GetPos(); }
	VECTOR GetLastPos(void) { return customers_.back()->GetPos(); }

	bool GetCustomerMove(void) { return customers_.front()->GetIsMove(); }

	bool CheckFirstCustomerCol(void);

private:
	// 静的インスタンス
	static CustomerManager* instance_;
	
	//とりあえず一人
	std::vector<std::shared_ptr<CustomerBase>> customers_;

	VECTOR prePos_;
};

