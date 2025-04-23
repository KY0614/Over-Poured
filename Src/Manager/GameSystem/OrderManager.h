//#pragma once
#include<vector>
#include<memory>

class Order;

class OrderManager
{
public:

	//注文関連
	static constexpr int MAX_CREATE_NUM = 5;	//最大注文生成数

	//インスタンスの生成
	static void CreateInstance(void);

	//インスタンスの取得
	static OrderManager& GetInstance(void);

	void Init(void);
	void Update(void);

	//リソースの破棄
	void Destroy(void);

	//注文を生成
	void InitCreateOrder(void);		//最初に５つ生成する用
	void CreateOrder(void);			//注文が１つ減った時に要素の最後に１つ生成する用

	void AddOrder(std::unique_ptr<Order> order);

	void ClearOrder(void);

private:
	//シングルトン用インスタンス
	static OrderManager* instance_;

	std::vector<std::unique_ptr<Order>> orders_;

};

