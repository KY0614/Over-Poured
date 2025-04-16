#pragma once
#include <memory>
#include "SceneBase.h"

class TutorialScene : public SceneBase
{
public:

	// コンストラクタ
	TutorialScene(void);

	// デストラクタ
	~TutorialScene(void);

	void Init(void) override;
	void Update(void) override;
	void Draw(void) override;

private:
};

