#pragma once
#include <memory>
#include "SceneBase.h"
#include "../Object/Common/Transform.h"

class SceneManager;
class AnimationController;
class ModelRenderer;
class ModelMaterial;


class TitleScene : public SceneBase
{

public:

	//コンストラクタ
	TitleScene(void);

	//デストラクタ
	~TitleScene(void);

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
	//マテリアル
	std::unique_ptr<ModelMaterial> material_;
	//レンダラ
	std::unique_ptr<ModelRenderer> renderer_;

	//画像
	int pushImg_;	//押下画像
	int titleImg_;	//ロゴ画像

	//ビュー切り替え
	bool isView_;	//true:表示,false:非表示
	//ハイライト経過時間
	float highlightTime_;

	//お店用モデル情報
	Transform cafeTran_;

	//地面用モデル情報
	Transform groundTran_;

	//キャラクターモデル情報
	Transform character_;

	//アニメーション
	std::unique_ptr<AnimationController> animationController_;

	/// <summary>
	/// モデル初期化
	/// </summary>
	void Init3DModel(void);

	/// <summary>
	/// マテリアル情報初期化
	/// </summary>
	/// <param name="">マテリアルの定数バッファ設定</param>
	void InitMaterial(void);

	/// <summary>
	/// 画像読み込み
	/// </summary>
	void LoadImages(void);

	/// <summary>
	/// サウンド初期化
	/// </summary>
	void InitSound(void);
};
