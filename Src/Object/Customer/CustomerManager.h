#pragma once
#include <vector>
#include <memory>
#include "./CustomerBase.h"
#include "../Order/Order.h"

class OrderUI;
class GaugeUI;

class CustomerManager
{
public:

	//お客の最大人数
	static constexpr int MAX_CUSTOMER_SIZE = 10;

	//生成する上限
	static constexpr int MAX_CREATE_SIZE = 6;

	//お客同士の間隔
	static constexpr float CUSTOMERS_SPACE = 90.0f;

	static constexpr float ORDER_UI_OFFSET_X = 130.0f;
	static constexpr float ORDER_UI_OFFSET_Y = 220.0f;
	
	static constexpr float TIMER_UI_OFFSET_X = 180.0f;
	static constexpr float TIMER_UI_OFFSET_Y = 260.0f;

	CustomerManager(void);
	~CustomerManager(void);

	void Init(void);
	void Update(float orderTime);
	void Draw(void);

	/// <summary>
	/// 最初の5人の座標を初期化
	/// </summary>
	void InitCustomersPos(void);	

	/// <summary>
	/// お客を一人生成する
	/// </summary>
	/// <param name="order">生成するタイプ</param>
	void CreateSingleCustomer(Order::OrderData data);

	/// <summary>
	/// お客を追加生成した後に位置調整する用
	/// </summary>
	void MoveCustomerPos(void);

	/// <summary>
	/// 配列の先頭のお客情報を削除
	/// </summary>
	void ClearFirstCustomers(void);

	/// <summary>
	/// 客を動かす
	/// </summary>
	void IsMove(void) { isMove_ = true; }

	/// <summary>
	/// 客を止める
	/// </summary>
	/// <param name="">falseにすると止まる</param>
	void IsNotMove(void) { isMove_ = false; }

	/// <summary>
	/// 客が動いているか取得する
	/// </summary>
	/// <param name=""></param>
	/// <returns>true:動いている　false:止まってる</returns>
	bool GetIsMove(void) { return isMove_; }

	/// <summary>
	/// スコアによるお客の反応の設定
	/// </summary>
	/// <param name="score">スコア</param>
	void SetCustomerReacton(int score);

	VECTOR SetLastCustomerPos(void);

	int GetCustomerNum(void) { return customers_.size(); }

	/// <summary>
	/// 先頭のお客がカウンター前にいるかどうか
	/// </summary>
	/// <param name=""></param>
	/// <returns>true:いる　false:いない</returns>
	bool CheckFirstCustomerCol(void);

	/// <summary>
	/// 2人目のお客がカウンター前にいるかどうか
	/// </summary>
	/// <param name="">先頭のお客の注文提供後に確認する用</param>
	/// <returns>true:いる　false:いない</returns>
	bool CheckSecondCustomerCol(void);

	/// <summary>
	/// 注文の確認UIを表示する
	/// </summary>
	/// <param name="index">指定する配列</param>
	/// <param name="isActive">表示させるかどうか</param>
	void IsCheckUI(const int index,const bool isActive);

private:
	// 静的インスタンス
	static CustomerManager* instance_;
	
	//お客生成用
	std::vector<std::unique_ptr<CustomerBase>> customers_;

	//注文UI用
	std::vector<std::unique_ptr<OrderUI>> orderUI_;

	bool isMove_;
	int cnt_;
};

