#pragma once
#include <memory>
#include "SceneBase.h"

class Score;

class ResultScene : public SceneBase
{
public:

	// �R���X�g���N�^
	ResultScene(void);

	// �f�X�g���N�^
	~ResultScene(void);

	void Init(void) override;
	void Update(void) override;
	void Draw(void) override;

private:
	//�X�R�A
	std::unique_ptr<Score> score_;

};

