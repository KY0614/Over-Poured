#pragma once
#include "SceneBase.h"

class TutorialScene : public SceneBase
{
public:

	//�R���X�g���N�^
	TutorialScene(void);

	//�f�X�g���N�^
	~TutorialScene(void);

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
	//�֐��|�C���^
	using UpdateFunc_t = void(TutorialScene::*)();
	using DrawFunc_t = void(TutorialScene::*)();

	UpdateFunc_t update_;
	DrawFunc_t draw_;

	void UpdateOperation(void);
	
	void DrawOperation(void);

	//�摜�Ő����p�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[

	//�����p�摜
	int* tutorialImgs_;
	int tutorialBackImg_;
	int* cursorImg_;

	int decoImg_;

	int pushImg_;	//�����摜

	//�����p�摜�̃C���f�b�N�X
	int imgIdx_;
	
	//�_�ŗp
	int blinkTime_;	//�_�Ŏ���
	int blinkIdx_;	//�J�[�\���摜�̓_�ŗp�C���f�b�N�X

	bool isBlinkR_;	//�E�J�[�\���̓_�Ńt���O
	bool isBlinkL_;	//���J�[�\���̓_�Ńt���O

	//�{�^�������p
	bool isPushButton_;	//true:�摜�\���@ false:��\��

	/// <summary>
	/// �摜�̓ǂݍ���
	/// </summary>
	void LoadImages(void);

	/// <summary>
	/// �T�E���h�̏���������
	/// </summary>
	void InitSound(void);

	/// <summary>
	/// �_�ł̐؂�ւ�
	/// </summary>
	void CursorHightlight(void);

	/// <summary>
	/// �y�[�W�؂�ւ��̏���
	/// </summary>
	void ProcessPageChange(void);
	
	/// <summary>
	/// �摜�̕`��
	/// </summary>
	void ImageDraw(void);
};

