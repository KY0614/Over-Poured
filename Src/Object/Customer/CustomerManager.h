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

	//�R���X�g���N�^
	CustomerManager(void);
	//�f�X�g���N�^
	~CustomerManager(void);

	/// <summary>
	/// ����������
	/// </summary>
	void Init(void);

	/// <summary>
	/// �X�V����
	/// </summary>
	/// <param name="orderTime">�����̌o�ߎ���</param>
	void Update(float orderTime);

	/// <summary>
	/// �`�揈��
	/// </summary>
	void Draw(void);

	/// <summary>
	/// ���q����l��������
	/// </summary>
	/// <param name="order">��������^�C�v</param>
	void CreateSingleCustomer(Order::OrderData data);

	/// <summary>
	/// �z��̐擪�̂��q�����폜
	/// </summary>
	void ClearFirstCustomers(void);

	/// <summary>
	/// �q�𓮂���
	/// </summary>
	void StartMove(void) { isCustomersMove_ = true; }

	/// <summary>
	/// �q���~�߂�
	/// </summary>
	/// <param name="">�~�߂邽�߂�false�ɂ���</param>
	void StopMove(void) { isCustomersMove_ = false; }

	/// <summary>
	/// �q�������Ă��邩�擾����
	/// </summary>
	/// <param name=""></param>
	/// <returns>true:�����Ă���@false:�~�܂��Ă�</returns>
	bool GetIsMove(void)const { return isCustomersMove_; }

	/// <summary>
	/// �X�R�A�ɂ�邨�q�̔����̐ݒ�
	/// </summary>
	/// <param name="score">�X�R�A</param>
	void SetCustomerReacton(int score);

	/// <summary>
	/// �Ō���i�U�l�ځj�̍��W���擾
	/// </summary>
	/// <param name="">�T�l�ڂ���Ԋu�������č��ɂ��炵�����W��Ԃ�</param>
	/// <returns>�Ō���̍��W</returns>
	VECTOR GetLastCustomerPos(void) const;

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

	bool isCustomersMove_;
	int cnt_;


	/// <summary>
	/// �ŏ���5�l�̍��W��������
	/// </summary>
	void InitCustomersPos(void);
};

