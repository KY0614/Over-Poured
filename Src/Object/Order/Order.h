#pragma once

class Order
{
public:
	//�����֘A
	static constexpr int ORDER_MAX_NUM = 2;			//�����̍ő吔
	static constexpr int ORDER_MIN_NUM = 1;			//�����̍ŏ���

	static constexpr float ONE_ORDER_TIME = 25.0f;	//�����̐�������(1��)
	static constexpr float TWO_ORDER_TIME = 33.0f;	//�����̐�������(�Q��)

	//���i�֘A
	static constexpr int DRINK_MAX_NUM = 2;			//���ݕ��ނ̍ő吔
	static constexpr int FOOD_MAX_NUM = 2;			//�H�ו��ނ̍ő吔

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
		int num_;			//������
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
	/// ���������擾
	/// </summary>
	/// <param name="">�������Ă��鏤�i�̐�</param>
	/// <returns>������</returns>
	int GetOrderNum(void) const { return orderNum_; }	

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

	//������
	int orderNum_;

	//�ݒ�֐�-------------------------------------------------

	/// <summary>
	/// ��������ݒ肷��
	/// </summary>
	/// <param name="num"></param>
	void SetOrderNum(int num) { orderData_.num_ = num; }

	/// <summary>
	/// �������e�̈��ݕ���ݒ肷��
	/// </summary>
	/// <param name="type">�ݒ肷�����</param>
	void SetDrink(DRINK type) { orderData_.drink_ = type; }

	/// <summary>
	/// �������e�̐H�ו���ݒ肷��
	/// </summary>
	/// <param name="type">�ݒ肷�����</param>
	void SetSweets(SWEETS type) { orderData_.sweets_ = type; };

	/// <summary>
	/// �����̐������Ԃ�ݒ肷��
	/// </summary>
	/// <param name="time">�ݒ肷�鎞��</param>
	void SetOrderTime(float time) { orderData_.time_ = time; }
	//--------------------------------------------------------------
};

