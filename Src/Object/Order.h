#pragma once

class Order
{
public:

	//�����֘A
	static constexpr int ORDER_MAX_NUM = 2;			//�ő咍�����e��

	static constexpr float ONE_ORDER_TIME = 3.5f;	//�����̐�������(1��)
	static constexpr float TWO_ORDER_TIME = 5.5f;	//�����̐�������(�Q��)

	//���i�֘A
	static constexpr int DRINK_MAX_NUM = 2;			//���ݕ��ނ̍ő吔
	static constexpr int FOOD_MAX_NUM = 2;			//�H�ו��ނ̍ő吔

	//�h�����N�̎��
	enum class DRINK
	{
		NONE,
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
		SWEETS sweets_;		//�X�C�[�c
		float time_;		//�����̐�������
	};

	//�R���X�g���N�^
	Order(void);

	//�f�X�g���N�^
	~Order(void);

	void Init(void);
	void Update(void);
	void Draw(void);

	//�����𐶐�����
	void CreateOrder(void);		//�P����

	//�ݒ肷��֐�-------------------------------------------------

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

	//�擾����֐�--------------------------------------------------------------------

	int GetOrderNum(void) { return orderNum_; }		//���������擾����

	OrderData GetOrder(void) { return orderData_; }			//�������e���擾����

	float GetOrderTime(void) { return orderData_.time_; }	//�����̐������Ԃ��擾����

private:
	//�������e
	OrderData orderData_;

	//������
	int orderNum_;

	//void UpdateImgGUI(void);
};

