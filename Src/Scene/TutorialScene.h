#pragma once
#include "SceneBase.h"

class TutorialScene : public SceneBase
{
public:

	//コンストラクタ
	TutorialScene(void);

	//デストラクタ
	~TutorialScene(void);

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
	//関数ポインタ
	using UpdateFunc_t = void(TutorialScene::*)();
	using DrawFunc_t = void(TutorialScene::*)();

	UpdateFunc_t update_;
	DrawFunc_t draw_;

	void UpdateOperation(void);
	
	void DrawOperation(void);

	//画像で説明用ーーーーーーーーーーーーーーーーーーーーーー

	//説明用画像
	int* tutorialImgs_;
	int tutorialBackImg_;
	int* cursorImg_;

	int decoImg_;

	int pushImg_;	//押下画像

	//説明用画像のインデックス
	int imgIdx_;
	
	//点滅用
	int blinkTime_;	//点滅時間
	int blinkIdx_;	//カーソル画像の点滅用インデックス

	bool isBlinkR_;	//右カーソルの点滅フラグ
	bool isBlinkL_;	//左カーソルの点滅フラグ

	//ボタン押下用
	bool isPushButton_;	//true:画像表示　 false:非表示

	/// <summary>
	/// 画像の読み込み
	/// </summary>
	void LoadImages(void);

	/// <summary>
	/// サウンドの初期化処理
	/// </summary>
	void InitSound(void);

	/// <summary>
	/// 点滅の切り替え
	/// </summary>
	void CursorHightlight(void);

	/// <summary>
	/// ページ切り替えの処理
	/// </summary>
	void ProcessPageChange(void);
	
	/// <summary>
	/// 画像の描画
	/// </summary>
	void ImageDraw(void);
};

