#pragma once
#include <DxLib.h>

class UIBase
{
public:
    //コンストラクタ
    UIBase(void);
    //デストラクタ
    virtual ~UIBase() = default;

    //初期化処理
    virtual void Init(void) = 0;
    //更新処理
    virtual void Update(void) = 0;
    //描画処理
    virtual void Draw(void) = 0;

    /// <summary>
    /// 座標を設定
    /// </summary>
    /// <param name="pos">表示する座標</param>
    void SetPos(const VECTOR pos) { pos_ = pos; }

	/// <summary>
	/// 透過度(α値)を設定
	/// </summary>
	/// <param name="alpha">透過度</param>
	void SetAlpha(const float alpha) { alpha_ = alpha; }

    /// <summary>
    /// 表示するかどうか
    /// </summary>
    /// <param name="isActive">true:表示　false:非表示</param>
    void SetActive(const bool isActive) { isActive_ = isActive; }

protected:

    //UI画像ハンドル
    int uiImg_;

    //UI座標
    VECTOR pos_;

    //UI画像の透過度
    float alpha_;

	//表示、非表示用のフラグ
    bool isActive_;
};

