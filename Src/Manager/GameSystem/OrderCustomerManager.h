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
	/// �������ꂽ�����̓��e�ɂ���Ă��q�̎�ނ����߂Đ�������
	/// </summary>
	/// <param name=""></param>
	void CreateCustomerByOrder(void);

private:

	//CustomerManger�̃|�C���^
	std::unique_ptr<CustomerManager> customerMng_;

	//OrderManager�̃|�C���^
	std::unique_ptr<OrderManager> orderMng_;

};

