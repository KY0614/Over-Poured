//#pragma once
#include<vector>
#include<memory>

class Order;

class OrderManager
{
public:

	//�����֘A
	static constexpr int MAX_CREATE_NUM = 5;	//�ő咍��������

	//�C���X�^���X�̐���
	static void CreateInstance(void);

	//�C���X�^���X�̎擾
	static OrderManager& GetInstance(void);

	void Init(void);
	void Update(void);

	//���\�[�X�̔j��
	void Destroy(void);

	//�����𐶐�
	void InitCreateOrder(void);		//�ŏ��ɂT��������p
	void CreateOrder(void);			//�������P���������ɗv�f�̍Ō�ɂP��������p

	void AddOrder(std::unique_ptr<Order> order);

	void ClearOrder(void);

private:
	//�V���O���g���p�C���X�^���X
	static OrderManager* instance_;

	std::vector<std::unique_ptr<Order>> orders_;

};

