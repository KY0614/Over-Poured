#pragma once
#include <memory>
#include "SceneBase.h"

class MovieScene : public SceneBase
{
public:

	// �R���X�g���N�^
	MovieScene(void);

	// �f�X�g���N�^
	~MovieScene(void);

	void Init(void) override;
	void Update(void) override;
	void Draw(void) override;

private:
};

