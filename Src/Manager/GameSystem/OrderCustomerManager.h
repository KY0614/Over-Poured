#pragma once
#include <memory>
#include"../../Object/Customer/CustomerBase.h"
#include "../../Object/Order/Order.h"

class OrderManager;
class CustomerManager;
class StageObject;

class OrderCustomerManager
{
public:
	//�����̃X�R�A
	static constexpr int DRINK_SCORE = 50;	//���ݕ��̃X�R�A
	static constexpr int SWEETS_SCORE = 30;	//�X�C�[�c�̃X�R�A
	static constexpr int LID_SCORE = 20;	//�X�C�[�c�̃X�R�A

	static constexpr int TIME_EARLY_BONUS = 50;	//�����񋟂̎��ԃ{�[�i�X
	static constexpr int TIME_BONUS = 10;		//���ʂ̒񋟂̎��ԃ{�[�i�X

	static constexpr float TIME_EARLY_LIMIT = 6.0f;	//�����񋟂̎��Ԑ���
	static constexpr float TIME_LIMIT = 3.0f;	//���ʒ񋟂̎��Ԑ���

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
	int GetOrderScore(const Order::OrderData serve);

	/// <summary>
	/// 
	/// </summary>
	/// <param name=""></param>
	void IsServe(void) { isServe_ = true; }

	/// <summary>
	/// �q�������Ă��邩�ǂ������擾����
	/// </summary>
	/// <returns>�����Ă���ꍇ��true�A�����łȂ��ꍇ��false</returns>
	bool GetIsMoving(void);

	Order::OrderData GetOrderData(void) const;

	void CheckServeAndOrder(const Order::OrderData serve);

private:

	//OrderManager�̃|�C���^
	std::unique_ptr<OrderManager> orderMng_;

	//CustomerManger�̃|�C���^
	std::shared_ptr<CustomerManager> customerMng_;

	bool isServe_;

	void DebugDraw(void);
};