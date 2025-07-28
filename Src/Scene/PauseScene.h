#pragma once
#include "SceneBase.h"

class PauseScene : public SceneBase
{
public:

	PauseScene(SceneManager& manager);
	~PauseScene(void);

	void Init(void)override;
	void Update(void)override;
	void Draw(void)override;

private:

	//ŠÖ”ƒ|ƒCƒ“ƒ^
	using UpdateFunc_t = void(PauseScene::*)();
	using DrawFunc_t = void(PauseScene::*)();

	UpdateFunc_t update_;
	DrawFunc_t draw_;
};

