#pragma once
#include "../SceneBase.h"

class KeyConfigScene : public SceneBase
{
public:

	//コンストラクタ
	KeyConfigScene(void);
	//デストラクタ
	~KeyConfigScene(void) override;

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
	int backImg_;		//背景
	int keyConfImg_;	//キー説明画像
	int menuBackImg_;	//メニューバック画像

	/// <summary>
	/// 画像の読み込み処理
	/// </summary>
	void LoadImages(void);

	/// <summary>
	/// サウンドの初期化処理
	/// </summary>
	void InitSound(void);
};