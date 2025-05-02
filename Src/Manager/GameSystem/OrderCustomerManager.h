#pragma once
#include <memory>

class OrderManager;
class CustomerManager;

class OrderCustomerManager
{
public:

	static constexpr float ORDER_INTERVAL_MAX = 3.5f;

	//コンストラクタ
	OrderCustomerManager(void);

	//デストラクタ
	~OrderCustomerManager(void);

	
	void Init(void);	//初期化処理
	void Update(void);	//更新処理
	void Draw(void);	//描画処理

	/// <summary>
	/// 生成された注文の内容に合わせてお客を生成する
	/// </summary>
	/// <param name=""></param>
	void CreateCustomersByOrders(void);

	/// <summary>
	/// お客と注文を追加で生成
	/// </summary>
	void AddOrdersAndCustomers(void);
	
	/// <summary>
	/// 先頭のお客と注文を削除する
	/// </summary>
	void ClearOrderAndCustomer(void);

	/// <summary>
	/// 追加で生成された注文の内容に合わせてお客を生成する
	/// </summary>
	void AddCustomerByOrder(void); 

	int CheckServeAndOrder(Order::OrderData serve);

	/// <summary>
	/// 生成しているお客の数
	/// </summary>
	/// <param name=""></param>
	/// <returns>お客の数</returns>
	int GetCustomerNum(void) const;

private:

	//OrderManagerのポインタ
	std::unique_ptr<OrderManager> orderMng_;

	//CustomerMangerのポインタ
	std::unique_ptr<CustomerManager> customerMng_;

	bool isCounter_;

	void DebugDraw(void);
};

