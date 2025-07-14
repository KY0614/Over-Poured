#pragma once
#include<vector>
#include<memory>
#include"../../Object/Order/Order.h"

class Order;

class OrderManager
{
public:

	//�����֘A
	static constexpr int MAX_CREATE_NUM = 6;	//�ő咍��������

	OrderManager(void);
	~OrderManager(void);

	void Init(void);
	void FirstOrderUpdate(void);
	void Draw(void);

	//�����𐶐�
	void InitOrder(void);			//�ŏ��ɂT��������p

	void CreateSingleOrder(void);	//�ⓚ���p�ň������������p

	void AddOrder(void);			//�������P��������P�ǉ���������p

	/// <summary>
	/// �ŏ��̒����̐������Ԃ��߂������ǂ�����Ԃ�
	/// </summary>
	/// <returns>true:�߂����@false:�߂��ĂȂ� </returns>
	bool IsFirstOrderTimeOut(void);

	//�������폜
	void ClearFirstOrder(void);

	//�ŏ��̒������e���擾
	Order::OrderData GetFirstOrder(void) { return orders_.front()->GetOrder(); }

	std::vector<Order::DRINK> GetAllOrderDrink(void) const;
	std::vector<Order::SWEETS> GetAllOrderSweet(void) const;
	std::vector<Order::OrderData> GetAllOrder(void) const;
	Order::DRINK GetLastOrderDrink(void) const;
	Order::SWEETS GetLastOrderSweets(void) const;
	Order::OrderData GetLastOrderData(void) const;

private:

	//�����Ǘ��p
	std::vector<std::unique_ptr<Order>> orders_;

};

