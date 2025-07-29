#pragma once
#include "SceneBase.h"

class PauseScene : public SceneBase
{
public:

	PauseScene(void);
	~PauseScene(void);

	void Init(void)override;
	void Update(void)override;
	void Draw(void)override;

private:

	//ä÷êîÉ|ÉCÉìÉ^
	using UpdateFunc_t = void(PauseScene::*)();
	using DrawFunc_t = void(PauseScene::*)();

	UpdateFunc_t update_;
	DrawFunc_t draw_;

	int backImg_;

	void UpdateNormal(void);
	void DrawNormal(void);
};

