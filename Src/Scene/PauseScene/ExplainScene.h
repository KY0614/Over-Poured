#pragma once
#include "../SceneBase.h"

class ExplainScene : public SceneBase
{
public:

	// コンストラクタ
	ExplainScene(void);
	// デストラクタ
	~ExplainScene(void);

	/// <summary>
	/// 初期化処理
	/// </summary>
	void Init(void) override;

	/// <summary>
	/// 更新処理
	/// </summary>
	void Update(void) override;

	/// <summary>
	/// 描画処理
	/// </summary>
	void Draw(void) override;

private:
	//画像
	int backImg_;		//背景画像
	int explainImg_;	//説明画像
	int menuBackImg_;	//メニューに戻る画像

	/// <summary>
	/// 画像の読み込み処理
	/// </summary>
	void LoadImages(void);

	/// <summary>
	/// サウンドの初期化処理
	/// </summary>
	void InitSound(void);
};

