#pragma once
#include <string>
#include "UIBase.h"

class PopUpUI : public UIBase
{
public:

    //コンストラクタ
    PopUpUI(int score, const VECTOR& startPos);
    //デストラクタ
    virtual ~PopUpUI() = default;

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
    //スコア
    int score_;

	//アニメーション時間経過用
    float animTime_;

    //スコアの数字画像ハンドル
    int* numbersImgs_;

    /// <summary>
    /// スコアをフェードアウトしながら描画する
    /// </summary>
    /// <param name="score">スコア</param>
    /// <param name="pos">描画座標</param>
    /// <param name="alpha">透明度</param>
    void DrawScore(int score, const VECTOR& pos, float alpha);
};

