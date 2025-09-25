#pragma once
#include<vector>
#include<memory>
#include"../../Object/Order/Order.h"

class Order;

class OrderManager
{
public:

	//�R���X�g���N�^
	OrderManager(void);

	//�f�X�g���N�^
	~OrderManager(void);

	/// <summary>
	/// ����������
	/// </summary>
	void Init(void);

	/// <summary>
	/// �擪�̒������X�V����
	/// </summary>
	/// <param name=""></param>
	void FirstOrderUpdate(void);
	
	//�����֐�-----------------------------------------------------

	/// <summary>
	/// ������ǉ�����
	/// </summary>
	/// <param name="">���������ő吔�����ɂȂ�����ǉ�����</param>
	void AddCreateOrder(void);

	//--------------------------------------------------------------
	//�폜�֐�------------------------------------------------------

	/// <summary>
	/// �擪�̒������폜����
	/// </summary>
	/// <param name=""></param>
	void ClearFirstOrder(void);

	//---------------------------------------------------------------
	//�擾�֐�-------------------------------------------------------

	/// <summary>
	/// �ŏ��̒����̐������Ԃ��߂������ǂ�����Ԃ�
	/// </summary>
	/// <returns>true:�߂����@false:�߂��ĂȂ� </returns>
	const bool& IsFirstOrderTimeOut(void);

	/// <summary>
	/// �擪�̒������e���擾����
	/// </summary>
	/// <param name=""></param>
	/// <returns></returns>
	const Order::OrderData& GetFirstOrder(void) { return orders_.front()->GetOrder(); }

	/// <summary>
	/// ���ׂĂ̒����f�[�^���擾����
	/// </summary>
	/// <param name=""></param>
	/// <returns>�������e�f�[�^</returns>
	const std::vector<Order::OrderData>& GetAllOrder(void) const;

	/// <summary>
	/// �Ō���̒����f�[�^���擾����
	/// </summary>
	/// <param name=""></param>
	/// <returns>�Ō���̒����f�[�^</returns>
	const Order::OrderData& GetLastOrderData(void) const;

	//----------------------------------------------------------------

private:

	//�����Ǘ��p
	std::vector<std::unique_ptr<Order>> orders_;

	//�����֐�-----------------------------------------------------

	/// <summary>
	/// �������ő吔�܂Ő�������
	/// </summary>
	/// <param name="">�Q�[���̎n�܂�ɐ�������p</param>
	void CreateOrderMaxNum(void);

	/// <summary>
	/// �P���������𐶐�����
	/// </summary>
	/// <param name="">Order�N���X���P�������A�z��Ɋi�[</param>
	void CreateSingleOrder(void);
};

