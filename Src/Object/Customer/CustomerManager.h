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

	//���q�̍ő�l��
	static constexpr int MAX_CUSTOMER_SIZE = 10;

	//����������
	static constexpr int MAX_CREATE_SIZE = 6;

	//���q���m�̊Ԋu
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
	/// �ŏ���5�l�̍��W��������
	/// </summary>
	void InitCustomersPos(void);	

	/// <summary>
	/// ���q����l��������
	/// </summary>
	/// <param name="order">��������^�C�v</param>
	void CreateSingleCustomer(Order::OrderData data);

	/// <summary>
	/// ���q��ǉ�����������Ɉʒu��������p
	/// </summary>
	void MoveCustomerPos(void);

	/// <summary>
	/// �z��̐擪�̂��q�����폜
	/// </summary>
	void ClearFirstCustomers(void);

	/// <summary>
	/// �q�𓮂���
	/// </summary>
	void IsMove(void) { isMove_ = true; }

	/// <summary>
	/// �q���~�߂�
	/// </summary>
	/// <param name="">false�ɂ���Ǝ~�܂�</param>
	void IsNotMove(void) { isMove_ = false; }

	/// <summary>
	/// �q�������Ă��邩�擾����
	/// </summary>
	/// <param name=""></param>
	/// <returns>true:�����Ă���@false:�~�܂��Ă�</returns>
	bool GetIsMove(void) { return isMove_; }

	/// <summary>
	/// �X�R�A�ɂ�邨�q�̔����̐ݒ�
	/// </summary>
	/// <param name="score">�X�R�A</param>
	void SetCustomerReacton(int score);

	VECTOR SetLastCustomerPos(void);

	int GetCustomerNum(void) { return customers_.size(); }

	/// <summary>
	/// �擪�̂��q���J�E���^�[�O�ɂ��邩�ǂ���
	/// </summary>
	/// <param name=""></param>
	/// <returns>true:����@false:���Ȃ�</returns>
	bool CheckFirstCustomerCol(void);

	/// <summary>
	/// 2�l�ڂ̂��q���J�E���^�[�O�ɂ��邩�ǂ���
	/// </summary>
	/// <param name="">�擪�̂��q�̒����񋟌�Ɋm�F����p</param>
	/// <returns>true:����@false:���Ȃ�</returns>
	bool CheckSecondCustomerCol(void);

	/// <summary>
	/// �����̊m�FUI��\������
	/// </summary>
	/// <param name="index">�w�肷��z��</param>
	/// <param name="isActive">�\�������邩�ǂ���</param>
	void IsCheckUI(const int index,const bool isActive);

private:
	// �ÓI�C���X�^���X
	static CustomerManager* instance_;
	
	//���q�����p
	std::vector<std::unique_ptr<CustomerBase>> customers_;

	//����UI�p
	std::vector<std::unique_ptr<OrderUI>> orderUI_;

	bool isMove_;
	int cnt_;
};

