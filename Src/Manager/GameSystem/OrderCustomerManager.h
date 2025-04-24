//#pragma once
#include <memory>

class OrderManager;
class CustomerManager;

class OrderCustomerManager
{
public:

	//コンストラクタ
	OrderCustomerManager(void);

	//デストラクタ
	~OrderCustomerManager(void);

	
	void Init(void);	//初期化処理
	void Update(void);	//更新処理
	void Draw(void);	//描画処理

	/// <summary>
	/// 生成された注文の内容によってお客の種類を決めて生成する
	/// </summary>
	/// <param name=""></param>
	void CreateCustomerByOrder(void);

private:

	//CustomerMangerのポインタ
	std::unique_ptr<CustomerManager> customerMng_;

	//OrderManagerのポインタ
	std::unique_ptr<OrderManager> orderMng_;

};

