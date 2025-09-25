#pragma once
#include <DxLib.h>
#include <vector>
#include <memory>

class UIBase;
class PopUpUI;

class UIManager
{
public:
	//インスタンスの生成
	static void CreateInstance(void);

	//インスタンスの取得
	static UIManager& GetInstance(void);

	//コンストラクタ
	UIManager(void);
	//デストラクタ
	~UIManager(void);

	/// <summary>
	/// 初期化処理
	/// </summary>
	void Init(void);

	/// <summary>
	/// 更新処理
	/// </summary>
	void Update(void);

	/// <summary>
	/// ポップアップUIの更新処理
	/// </summary>
	void PopUpUIUpdate(void);

	/// <summary>
	/// 描画処理
	/// </summary>
	void Draw(void);

	/// <summary>
	/// 解放処理
	/// </summary>
	void Release(void);

	/// <summary>
	/// リソースの破棄
	/// </summary>
	void Destroy(void);

	/// <summary>
	/// UIの追加
	/// </summary>
	/// <typeparam name="T">UIBaseを継承しているサブクラス</typeparam>
	/// <param name="ui">追加するUI</param>
	template <typename T>
	void AddUI(std::shared_ptr<T> ui)
	{
		//UIを追加
		uis_.emplace_back(ui);
	}

	/// <summary>
	/// ポップアップUIの追加(スコア用）
	/// </summary>
	/// <param name="score">スコア</param>
	/// <param name="pos">表示する座標</param>
	void AddPopUpUI(const int score, const VECTOR& pos);

private:
	//シングルトン用インスタンス
	static UIManager* instance_;

	//追加されたUIのリスト
	std::vector<std::weak_ptr<UIBase>> uis_;

	//ポップアップUIのリスト
	std::vector<std::unique_ptr<PopUpUI>> popUpUIs_;
};
