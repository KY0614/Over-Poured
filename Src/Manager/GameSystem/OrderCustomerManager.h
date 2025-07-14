#pragma once
#include <memory>
#include"../../Object/Customer/CustomerBase.h"
#include "../../Object/Order/Order.h"

class OrderManager;
class CustomerManager;
class StageObject;
class OrderUI;

class OrderCustomerManager
{
public:

	static constexpr float ORDER_INTERVAL_MAX = 3.5f;

	static constexpr float ORDER_UI_OFFSET_X = 130.0f;
	static constexpr float ORDER_UI_OFFSET_Y = 220.0f;

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

	/// <summary>
	/// 提供されたものと注文を比較してスコアを返す
	/// </summary>
	/// <param name="serve">提供されたもの</param>
	/// <returns>スコア</returns>
	int CheckServeAndOrder(const Order::OrderData serve);

	void IsServe(void) { isServe_ = true; }

	bool GetIsMoving(void);

	/// <summary>
	/// 生成しているお客の数
	/// </summary>
	/// <param name=""></param>
	/// <returns>お客の数</returns>
	int GetCustomerNum(void) const;

	Order::OrderData GetOrderData(void) const;

private:

	//OrderManagerのポインタ
	std::unique_ptr<OrderManager> orderMng_;

	//CustomerMangerのポインタ
	std::shared_ptr<CustomerManager> customerMng_;

	//std::vector<std::unique_ptr<OrderUI>> orderUI_;

	bool isServe_;

	void DebugDraw(void);
};