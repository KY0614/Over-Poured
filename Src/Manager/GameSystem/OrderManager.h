#pragma once
#include<vector>
#include<memory>
#include"../../Object/Order.h"

class Order;

class OrderManager
{
public:

	//注文関連
	static constexpr int MAX_CREATE_NUM = 5;	//最大注文生成数

	OrderManager(void);
	~OrderManager(void);

	void Init(void);
	void Update(void);
	void Draw(void);

	//注文を生成
	void InitOrder(void);			//最初に５つ生成する用

	void CreateSingleOrder(void);	//問答無用で一つだけ生成する用

	void AddOrder(void);			//注文が１つ減ったら１つ追加生成する用

	//注文を削除
	void ClearOrder(void);

	//最初の注文内容を取得
	Order::OrderData GetFirstOrder(void) { return orders_.front()->GetOrder(); }

	Order::DRINK GetDrink(void) { for (auto& o : orders_) { return o->GetOrder().drink_; } }

	int GetCount(void) { return count_; }

private:
	//シングルトン用インスタンス
	static OrderManager* instance_;

	//注文管理用
	std::vector<std::unique_ptr<Order>> orders_;

#ifdef _DEBUG

	int count_;

#endif // _DEBUG

};

