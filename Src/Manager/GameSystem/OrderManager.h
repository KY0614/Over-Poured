//#pragma once


class OrderManager
{
public:

	//�����֘A
	static constexpr int ORDER_MAX_NUM = 2;		//�ő咍����

	static constexpr float ONE_ORDER_TIME = 3.5f;	//�����̐�������(1��)
	static constexpr float TWO_ORDER_TIME = 5.5f;	//�����̐�������(�Q��)

	//���i�֘A
	static constexpr int DRINK_MAX_NUM = 2;		//���ݕ��ނ̍ő吔
	static constexpr int FOOD_MAX_NUM = 2;		//�H�ו��ނ̍ő吔

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
	struct Order
	{
		DRINK drink_;
		SWEETS sweets_;
		float orderTime_;
	};

	//�C���X�^���X�̐���
	static void CreateInstance(void);

	//�C���X�^���X�̎擾
	static OrderManager& GetInstance(void);

	void Init(void);
	void Update(void);

	//���\�[�X�̔j��
	void Destroy(void);

	//�����𐶐�
	Order CreateOrder(void);

	//�ݒ肷��֐�-------------------------------------------------

	/// <summary>
	/// ���ݕ��̓��e��ݒ肷��
	/// </summary>
	/// <param name="type">�ݒ肷�����</param>
	void SetDrink(DRINK type) { order_.drink_ = type; }

	/// <summary>
	/// �H�ו��̓��e��ݒ肷��
	/// </summary>
	/// <param name="type">�ݒ肷�����</param>
	void SetSweets(SWEETS type) { order_.sweets_ = type; };

	/// <summary>
	/// �����̐������Ԃ�ݒ肷��
	/// </summary>
	/// <param name="time">�ݒ肷�鎞��</param>
	void SetOrderTime(float time) { order_.orderTime_ = time; }

	//--------------------------------------------------------------

	//�擾����֐�--------------------------------------------------------------------
	
	int GetOrderNum(void) { return orderNum_; }		//���������擾����

	Order GetOrder(void) { return order_; }			//�������e���擾����

	float GetOrderTime(void) { return order_.orderTime_; }	//�����̐������Ԃ��擾����

private:
	//�V���O���g���p�C���X�^���X
	static OrderManager* instance_;

	Order order_;
	int orderNum_;
};

