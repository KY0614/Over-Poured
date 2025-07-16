#pragma once
#include <vector>
#include <memory>
#include <map>

class OrderUI;
class GaugeUI;
class IconUI;

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
	void Release(void);

	//���\�[�X�̔j��
	void Destroy(void);

	void AddGaugeUI(GaugeUI* ui);
	void AddOrderUI(OrderUI* ui);
	void AddIconUI(IconUI* ui);

private:
	//�V���O���g���p�C���X�^���X
	static UIManager* instance_;

	std::vector<GaugeUI*> gaugeUIs_;
	std::vector<OrderUI*> orderUIs_;
	std::vector<IconUI*> iconUIs_;
};

