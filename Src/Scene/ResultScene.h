#pragma once
#include <memory>
#include "SceneBase.h"

class ResultScene : public SceneBase
{
public:

	static constexpr int ADD_SCORE_SPEED = 8;

	// コンストラクタ
	ResultScene(void);

	// デストラクタ
	~ResultScene(void);

	void Init(void) override;
	void Update(void) override;
	void Draw(void) override;

private:

	int currentScr_;
};

