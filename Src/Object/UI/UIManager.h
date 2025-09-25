#pragma once
#include <DxLib.h>
#include <vector>
#include <memory>

class UIBase;
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

	/// <summary>
	/// UI�̒ǉ�
	/// </summary>
	/// <typeparam name="T">UIBase���p�����Ă���T�u�N���X</typeparam>
	/// <param name="ui">�ǉ�����UI</param>
	template <typename T>
	void AddUI(std::shared_ptr<T> ui)
	{
		//UI��ǉ�
		uis_.emplace_back(ui);
	}

	/// <summary>
	/// �|�b�v�A�b�vUI�̒ǉ�(�X�R�A�p�j
	/// </summary>
	/// <param name="score">�X�R�A</param>
	/// <param name="pos">�\��������W</param>
	void AddPopUpUI(const int score, const VECTOR& pos);

private:
	//�V���O���g���p�C���X�^���X
	static UIManager* instance_;

	//�ǉ����ꂽUI�̃��X�g
	std::vector<std::weak_ptr<UIBase>> uis_;

	//�|�b�v�A�b�vUI�̃��X�g
	std::vector<std::unique_ptr<PopUpUI>> popUpUIs_;
};
