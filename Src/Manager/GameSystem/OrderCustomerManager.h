//#pragma once
#include <memory>

class OrderManager;
class CustomerManager;

class OrderCustomerManager
{
public:

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

	void AddOrdersAndCustomers(void);

	/// <summary>
	/// �ǉ��Ő������ꂽ�����̓��e�ɍ��킹�Ă��q�𐶐�����
	/// </summary>
	/// <param name=""></param>
	void AddCustomerByOrder(void); 

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

	void DebugDraw(void);
};

