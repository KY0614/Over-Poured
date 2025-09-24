#pragma once
#include <vector>
#include <memory>
#include <map>

class UIBase;
class OrderUI;
class GaugeUI;
class IconUI;
class PopUpUI;

class UIManager
{
public:
	//�C���X�^���X�̐���
	static void CreateInstance(void);

	//�C���X�^���X�̎擾
	static UIManager& GetInstance(void);

	//�R���X�g���N�^
	UIManager(void);
	//�f�X�g���N�^
	~UIManager(void);

	/// <summary>
	/// ����������
	/// </summary>
	void Init(void);

	/// <summary>
	/// �X�V����
	/// </summary>
	void Update(void);

	/// <summary>
	/// �|�b�v�A�b�vUI�̍X�V����
	/// </summary>
	void PopUpUIUpdate(void);

	/// <summary>
	/// �`�揈��
	/// </summary>
	void Draw(void);

	/// <summary>
	/// �������
	/// </summary>
	void Release(void);

	/// <summary>
	/// ���\�[�X�̔j��
	/// </summary>
	void Destroy(void);

	template <typename T>
	void AddUI(std::shared_ptr<T> ui)
	{
		//UI��ǉ�
		uis_.emplace_back(ui);
	}

	void AddGaugeUI(GaugeUI* ui);
	void AddOrderUI(OrderUI* ui);
	void AddIconUI(IconUI* ui);
	void AddPopUpUI(int score, const VECTOR& pos);


private:
	//�V���O���g���p�C���X�^���X
	static UIManager* instance_;

	std::vector<std::weak_ptr<UIBase>> uis_;

	std::vector<GaugeUI*> gaugeUIs_;
	std::vector<OrderUI*> orderUIs_;
	std::vector<IconUI*> iconUIs_;
	std::vector<std::unique_ptr<PopUpUI>> popUpUIs_;
};
