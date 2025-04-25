//#pragma once
#include <memory>

class OrderManager;
class CustomerManager;

class OrderCustomerManager
{
public:

	//�R���X�g���N�^
	OrderCustomerManager(OrderManager& orderMng);

	//�f�X�g���N�^
	~OrderCustomerManager(void);

	
	void Init(void);	//����������
	void Update(void);	//�X�V����
	void Draw(void);	//�`�揈��

	/// <summary>
	/// �������ꂽ�����̓��e�ɍ��킹�Ă��q�𐶐�����
	/// </summary>
	/// <param name=""></param>
	void CreateCustomerByOrder(void);

	/// <summary>
	/// �ǉ��Ő������ꂽ�����̓��e�ɍ��킹�Ă��q�𐶐�����
	/// </summary>
	/// <param name=""></param>
	void AddCustomerByOrder(void);

	void ClearFirstCustomer(void);

	/// <summary>
	/// �������Ă��邨�q�̐�
	/// </summary>
	/// <param name=""></param>
	/// <returns>���q�̐�</returns>
	int GetCustomerNum(void) const;

private:

	//�Q��
	OrderManager& orderMng_;

	//CustomerManger�̃|�C���^
	std::unique_ptr<CustomerManager> customerMng_;

};

