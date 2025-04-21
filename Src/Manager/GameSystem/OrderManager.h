//#pragma once


class OrderManager
{
public:

	//�C���X�^���X�̐���
	static void CreateInstance(void);

	//�C���X�^���X�̎擾
	static OrderManager& GetInstance(void);

	void Init(void);
	void Update(void);

	//���\�[�X�̔j��
	void Destroy(void);

private:
	//�V���O���g���p�C���X�^���X
	static OrderManager* instance_;

};

