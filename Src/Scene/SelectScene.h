#pragma once
#include <memory>
#include "SceneBase.h"

class SelectScene : public SceneBase
{
public:

	// �R���X�g���N�^
	SelectScene(void);

	// �f�X�g���N�^
	~SelectScene(void);

	void Init(void) override;
	void Update(void) override;
	void Draw(void) override;

private:
	//���j���[�̕����񃊃X�g
	std::vector<std::wstring> selectList_;

	int currentIdx_;

	void DebugDraw(void);
};

