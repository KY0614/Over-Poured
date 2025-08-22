#pragma once
#include "../SceneBase.h"

class ExplainScene : public SceneBase
{
public:

	// �R���X�g���N�^
	ExplainScene(void);

	// �f�X�g���N�^
	~ExplainScene(void);

	// ����������
	void Init(void)override;

	// �X�V�X�e�b�v
	void Update(void)override;

	// �`�揈��
	void Draw(void)override;

private:

	int backImg_;
	int explainImg_;
	int menuBackImg_;
};

