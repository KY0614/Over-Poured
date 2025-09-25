#include "../Manager/Generic/ResourceManager.h"
#include "PopUpUI.h"

namespace
{
    //アニメーション時間（秒）
    const float ANIM_DURATION = 1.0f;
	//移動間隔(移動距離)
    const float MOVE_DISTANCE = 1.0f;
    //最大透過度
	const float ALPHA_MAX = 255.0f; 
    //フレームレート
	const float FPS_RATE = 60.0f; 
}

PopUpUI::PopUpUI(int score, const VECTOR& startPos) : score_(score), animTime_(0.0f)
{
    pos_ = startPos;
    alpha_ = ALPHA_MAX; //初期透過度を最大に設定
    isActive_ = true;
    numbersImgs_ = nullptr;
}

void PopUpUI::Init(void)
{
	//スコアの数字画像ハンドルを取得
    numbersImgs_ = ResourceManager::GetInstance().Load(ResourceManager::SRC::SCORE_NUMBER).handleIds_;
}

void PopUpUI::Update(void)
{
    if (!isActive_) return;

    animTime_ += 1.0f / FPS_RATE; //60FPSを想定した経過時間更新
    if (animTime_ >= ANIM_DURATION)
    {
        isActive_ = false; //アニメーション終了
        return;
    }

    // イージング関数を使用して位置と透過度を計算
    float t = animTime_ / ANIM_DURATION;
    pos_.y = pos_.y - t * MOVE_DISTANCE; //上に移動
    alpha_ = ALPHA_MAX * (MOVE_DISTANCE - t); //フェードアウト

	//透過度を設定
    SetAlpha(alpha_);
}

void PopUpUI::Draw(void)
{
	//表示されていなければ描画しない
    if (!isActive_) return;
	//スコアを描画
    DrawScore(score_, pos_, alpha_);
}

void PopUpUI::DrawScore(int score, const VECTOR& pos, float alpha)
{
    std::string str = std::to_string(score);
	const int digitWidth = 35;  //数字画像の幅(1つの数字の幅）
    SetDrawBlendMode(DX_BLENDMODE_ALPHA, static_cast<int>(alpha));
	const float scale = 0.4f; //数字画像の拡大率

    for (int i = 0; i < str.size(); ++i)
    {
        char ch = str[i];
		//'0'から'9'の範囲内の文字か確認
        if ('0' <= ch && ch <= '9')
        {
            int digit = ch - '0';
            DrawRotaGraph(
                pos.x - (str.size() * digitWidth / 2) + i * digitWidth,
                pos.y,
                scale, 0.0f,
                numbersImgs_[digit], true
            );
        }
    }
    SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0); //描画モードを元に戻す
}
