#pragma once
#include "../../Manager/Generic/ResourceManager.h"
#include "UIBase.h"

class IconUI : public UIBase
{
public:
    /// <summary>
    /// コンストラクタ
    /// </summary>
    /// <param name="localPos">ローカル座標</param>
    /// <param name="parentPos">追従先の座標</param>
    /// <param name="src">使用するソース</param>
    IconUI(const VECTOR localPos,
        const VECTOR parentPos,
        const ResourceManager::SRC src);
	//デストラクタ
    ~IconUI() override = default;

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

    /// <summary>
	/// UIのサイズを設定
    /// </summary>
    /// <param name="size">UIのサイズ</param>
    void SetUISize(const float size) { size_ = size; }

protected:
	//ローカル座標
    VECTOR localPos_;
	//追従先の座標
    VECTOR parentPos_;
    //サイズ
    float size_;
	//画像のソース
    ResourceManager::SRC imgSrc_;
};

