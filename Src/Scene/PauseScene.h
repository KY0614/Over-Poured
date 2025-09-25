#pragma once
#include "SceneBase.h"

class PauseScene : public SceneBase
{
public:
	//�|�[�Y���j���[���o������܂ł̃t���[����
	static constexpr int APPEAR_INTERVAL = 15;

	//�R���X�g���N�^
	PauseScene(void);
	//�f�X�g���N�^
	~PauseScene(void);

	/// <summary>
	/// ����������
	/// </summary>
	void Init(void) override;

	/// <summary>
	/// �X�V����
	/// </summary>
	void Update(void) override;

	/// <summary>
	/// �`�揈��
	/// </summary>
	void Draw(void) override;

private:

	//���j���[�̕����񃊃X�g
	std::vector<std::wstring> menuList_;
	//���j���[�ɑΉ�����֐��e�[�u��
	using MenuFunc_t = std::function<void()>;
	std::map<std::wstring, MenuFunc_t> menuFuncTable_;

	//�֐��|�C���^
	using UpdateFunc_t = void(PauseScene::*)();
	using DrawFunc_t = void(PauseScene::*)();
	//���݂̍X�V�֐�
	UpdateFunc_t update_;
	DrawFunc_t draw_;

	//�摜�n���h��
	int backImg_;	//�w�i�摜
	int* menuListImg_;	//���j���[���X�g�摜
	int menuCursorImg_;	//���j���[�J�[�\���摜

	//�|�[�Y���j���[���o��������܂ł̃t���[����
	int frame_;

	//�J�[�\���̃C���f�b�N�X
	int cursorIdx_;

	/// <summary>
	/// �o�����̍X�V����
	/// </summary>
	void UpdateAppear(void);

	/// <summary>
	/// �������̍X�V����
	/// </summary>
	void UpdateDisappear(void);

	/// <summary>
	/// �o�����̕`�揈��
	/// </summary>
	void DrawProcess(void);

	/// <summary>
	/// �|�[�Y���̍X�V����
	/// </summary>
	void UpdateNormal(void);

	/// <summary>
	/// �|�[�Y���̕`�揈��
	/// </summary>
	void DrawNormal(void);

	/// <summary>
	/// ���j���[���X�g�̕`��
	/// </summary>
	/// <param name=""></param>
	void DrawMenuList(void);

	/// <summary>
	/// �摜�̓ǂݍ���
	/// </summary>
	void LoadImages(void);

	/// <summary>
	/// �T�E���h�̏�����
	/// </summary>
	void InitSound(void);
};

