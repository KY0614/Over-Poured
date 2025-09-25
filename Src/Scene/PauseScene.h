#pragma once
#include "SceneBase.h"

class PauseScene : public SceneBase
{
public:
	//ポーズメニューが出現するまでのフレーム数
	static constexpr int APPEAR_INTERVAL = 15;

	//コンストラクタ
	PauseScene(void);
	//デストラクタ
	~PauseScene(void);

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

	//メニューの文字列リスト
	std::vector<std::wstring> menuList_;
	//メニューに対応する関数テーブル
	using MenuFunc_t = std::function<void()>;
	std::map<std::wstring, MenuFunc_t> menuFuncTable_;

	//関数ポインタ
	using UpdateFunc_t = void(PauseScene::*)();
	using DrawFunc_t = void(PauseScene::*)();
	//現在の更新関数
	UpdateFunc_t update_;
	DrawFunc_t draw_;

	//画像ハンドル
	int backImg_;	//背景画像
	int* menuListImg_;	//メニューリスト画像
	int menuCursorImg_;	//メニューカーソル画像

	//ポーズメニューを出現させるまでのフレーム数
	int frame_;

	//カーソルのインデックス
	int cursorIdx_;

	/// <summary>
	/// 出現中の更新処理
	/// </summary>
	void UpdateAppear(void);

	/// <summary>
	/// 消失中の更新処理
	/// </summary>
	void UpdateDisappear(void);

	/// <summary>
	/// 出現中の描画処理
	/// </summary>
	void DrawProcess(void);

	/// <summary>
	/// ポーズ中の更新処理
	/// </summary>
	void UpdateNormal(void);

	/// <summary>
	/// ポーズ中の描画処理
	/// </summary>
	void DrawNormal(void);

	/// <summary>
	/// メニューリストの描画
	/// </summary>
	/// <param name=""></param>
	void DrawMenuList(void);

	/// <summary>
	/// 画像の読み込み
	/// </summary>
	void LoadImages(void);

	/// <summary>
	/// サウンドの初期化
	/// </summary>
	void InitSound(void);
};

