#pragma once
#include <memory>
#include "SceneBase.h"

class TutorialScene : public SceneBase
{
public:

	// �R���X�g���N�^
	TutorialScene(void);

	// �f�X�g���N�^
	~TutorialScene(void);

	void Init(void) override;
	void Update(void) override;
	void Draw(void) override;

private:
};

