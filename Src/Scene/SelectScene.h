#pragma once
#include <memory>
#include "SceneBase.h"

class SelectScene : public SceneBase
{
public:

	// コンストラクタ
	SelectScene(void);

	// デストラクタ
	~SelectScene(void);

	void Init(void) override;
	void Update(void) override;
	void Draw(void) override;

private:
};

