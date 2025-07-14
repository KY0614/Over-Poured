#pragma once
#include <vector>
#include <memory>


class OrderUI;
class GaugeUI;

class UIManager
{
public:
	//�C���X�^���X�̐���
	static void CreateInstance(void);

	//�C���X�^���X�̎擾
	static UIManager& GetInstance(void);

	UIManager(void);
	~UIManager(void);

	void Init(void);
	void Update(void);
	void Draw(void);

	//���\�[�X�̔j��
	void Destroy(void);

	void AddGaugeUI(GaugeUI* ui);
	void AddOrderUI(OrderUI* ui);

private:
	//�V���O���g���p�C���X�^���X
	static UIManager* instance_;

	std::vector<GaugeUI*> gaugeUIs_;
	std::vector<OrderUI*> orderUIs_;
};

