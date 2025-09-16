#pragma once

class Order
{
public:

	//�h�����N�̎��
	enum class DRINK
	{
		NONE,	//����
		HOT,	//�z�b�g
		ICE,	//�A�C�X
	};

	//�X�C�[�c�̎��
	enum class SWEETS
	{
		NONE,		//����
		CHOCO,		//�`���R
		STRAWBERRY,	//�X�g���x���[
	};

	//�����̓��e
	struct OrderData
	{
		int orderNum_;		//������
		DRINK drink_;		//���ݕ�
		bool lid_;			//���ݕ��ɊW�����邩�ǂ���
		SWEETS sweets_;		//�X�C�[�c
		float time_;		//�����̐�������
	};

	//�R���X�g���N�^
	Order(void);

	//�f�X�g���N�^
	~Order(void);

	/// <summary>
	/// �������Ԃ̍X�V����
	/// </summary>
	void TimerUpdate(void);

	/// <summary>
	/// �����𐶐�
	/// </summary>
	/// <param name="">�P����</param>
	void CreateOrder(void);

	//�擾�֐�--------------------------------------------------------------------

	/// <summary>
	/// �������e���擾
	/// </summary>
	/// <param name="">�������e��Ԃ�(OrderData��Ԃ�)</param>
	/// <returns>�������e</returns>
	OrderData GetOrder(void) const { return orderData_; }

	/// <summary>
	/// �����̐������Ԃ��擾
	/// </summary>
	/// <param name=""></param>
	/// <returns></returns>
	float GetOrderTime(void) const { return orderData_.time_; }	

private:
	//�������e
	OrderData orderData_;

	//�ݒ�֐�-------------------------------------------------

	/// <summary>
	/// ��������ݒ肷��
	/// </summary>
	/// <param name="num"></param>
	void SetOrderNum(const int num) { orderData_.orderNum_ = num; }

	/// <summary>
	/// �������e�̈��ݕ���ݒ肷��
	/// </summary>
	/// <param name="type">�ݒ肷�����</param>
	void SetDrink(const DRINK type) { orderData_.drink_ = type; }

	/// <summary>
	/// �������e�̐H�ו���ݒ肷��
	/// </summary>
	/// <param name="type">�ݒ肷�����</param>
	void SetSweets(const SWEETS type) { orderData_.sweets_ = type; };

	/// <summary>
	/// �����̐������Ԃ�ݒ肷��
	/// </summary>
	/// <param name="time">�ݒ肷�鎞��</param>
	void SetOrderTime(const float time) { orderData_.time_ = time; }
};

