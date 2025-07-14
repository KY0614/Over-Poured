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

	/// <summary>
	/// �񋟂��ꂽ���̂ƒ������r���ăX�R�A��Ԃ�
	/// </summary>
	/// <param name="serve">�񋟂��ꂽ����</param>
	/// <returns>�X�R�A</returns>
	int CheckServeAndOrder(const Order::OrderData serve);

	void IsServe(void) { isServe_ = true; }

	bool GetIsMoving(void);

	/// <summary>
	/// �������Ă��邨�q�̐�
	/// </summary>
	/// <param name=""></param>
	/// <returns>���q�̐�</returns>
	int GetCustomerNum(void) const;

	Order::OrderData GetOrderData(void) const;

private:

	//OrderManager�̃|�C���^
	std::unique_ptr<OrderManager> orderMng_;

	//CustomerManger�̃|�C���^
	std::shared_ptr<CustomerManager> customerMng_;

	//std::vector<std::unique_ptr<OrderUI>> orderUI_;

	bool isServe_;

	void DebugDraw(void);
};