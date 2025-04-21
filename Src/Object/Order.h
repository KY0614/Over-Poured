//#pragma once

class Order
{
public:

	//�R���X�g���N�^
	Order(void);

	//�f�X�g���N�^
	~Order(void);

	void Init(void);
	void Update(void);
	void Draw(void);

	/// <summary>
	/// �����ɑ΂��Đ������Ԃ�ݒ肷��
	/// </summary>
	/// <param name="time">�ݒ肷�鐧������</param>
	void SetTimer(float time) { timer_ = time; }

	float GetTimer(void) { return timer_; }		//�������Ԃ��擾����

private:

	//�������ԗp
	float timer_;

	void UpdateImgGUI(void);
};

