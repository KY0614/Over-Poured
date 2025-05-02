#pragma once
#include <memory>

class OrderManager;
class CustomerManager;

class OrderCustomerManager
{
public:

	static constexpr float ORDER_INTERVAL_MAX = 3.5f;

	//�R���X�g���N�^
	OrderCustomerManager(void);

	//�f�X�g���N�^
	~OrderCustomerManager(void);

	
	void Init(void);	//����������
	void Update(void);	//�X�V����
	void Draw(void);	//�`�揈��

	/// <summary>
	/// �������ꂽ�����̓��e�ɍ��킹�Ă��q�𐶐�����
	/// </summary>
	/// <param name=""></param>
	void CreateCustomersByOrders(void);

	/// <summary>
	/// ���q�ƒ�����ǉ��Ő���
	/// </summary>
	void AddOrdersAndCustomers(void);
	
	/// <summary>
	/// �擪�̂��q�ƒ������폜����
	/// </summary>
	void ClearOrderAndCustomer(void);

	/// <summary>
	/// �ǉ��Ő������ꂽ�����̓��e�ɍ��킹�Ă��q�𐶐�����
	/// </summary>
	void AddCustomerByOrder(void); 

	int CheckServeAndOrder(Order::OrderData serve);

	/// <summary>
	/// �������Ă��邨�q�̐�
	/// </summary>
	/// <param name=""></param>
	/// <returns>���q�̐�</returns>
	int GetCustomerNum(void) const;

private:

	//OrderManager�̃|�C���^
	std::unique_ptr<OrderManager> orderMng_;

	//CustomerManger�̃|�C���^
	std::unique_ptr<CustomerManager> customerMng_;

	bool isCounter_;

	void DebugDraw(void);
};

