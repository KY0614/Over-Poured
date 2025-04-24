#pragma once
#include<vector>
#include<memory>
#include"../../Object/Order.h"

class Order;

class OrderManager
{
public:

	//�����֘A
	static constexpr int MAX_CREATE_NUM = 5;	//�ő咍��������

	OrderManager(void);
	~OrderManager(void);

	void Init(void);
	void Update(void);
	void Draw(void);

	//�����𐶐�
	void InitOrder(void);			//�ŏ��ɂT��������p

	void CreateSingleOrder(void);	//�ⓚ���p�ň������������p

	void AddOrder(void);			//�������P��������P�ǉ���������p

	//�������폜
	void ClearOrder(void);

	//�ŏ��̒������e���擾
	Order::OrderData GetFirstOrder(void) { return orders_.front()->GetOrder(); }

	Order::DRINK GetDrink(void) { for (auto& o : orders_) { return o->GetOrder().drink_; } }

	int GetCount(void) { return count_; }

private:
	//�V���O���g���p�C���X�^���X
	static OrderManager* instance_;

	//�����Ǘ��p
	std::vector<std::unique_ptr<Order>> orders_;

#ifdef _DEBUG

	int count_;

#endif // _DEBUG

};

