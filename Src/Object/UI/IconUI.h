#pragma once
#include "../../Manager/Generic/ResourceManager.h"
#include "UIBase.h"

class IconUI : public UIBase
{
public:
    //コンストラクタ
    IconUI(VECTOR localPos,VECTOR parentPos,ResourceManager::SRC src);
	//デストラクタ
    ~IconUI() = default;

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
    /// <param name="size"></param>
    void SetUISize(const float size) { size_ = size; }

protected:
	//ローカル座標
    VECTOR localPos_;
	//追従先の座標
    VECTOR parentPos_;
    //サイズ
    float size_;

    ResourceManager::SRC imgSrc_;
};

