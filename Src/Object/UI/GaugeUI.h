#pragma once
#include "UIBase.h"

class GaugeUI : public UIBase
{
public:
	//デフォルトのUIサイズ
	static constexpr float UI_DEFAULT_WIDTH = 80.0f;	//幅
	static constexpr float UI_DEFAULT_HEIGHT = 10.0f;	//高さ

	//コンストラクタ
    GaugeUI(const bool isCircle, const float activeTime);
	//デストラクタ
	~GaugeUI(void)override = default;

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
	/// <param name="width"></param>
	/// <param name="height"></param>
	void SetUISize(const float width, const float height) { width_ = width; height_ = height; }

	/// <summary>
	/// UIの表示をリセット
	/// </summary>
	void Reset(void);

private:
	//影の画像ID
	int shadowImg_;

	//円形の画像ID
	int circleImg_;
	
	//円形の影画像ID
	int cShadowImg_;	
	
	//最大表示時間
    float activeTime_;
	
	//経過時間
    float currentTime_;
	
	//true: 円形, false: 長方形
    bool isCircle_;     

	//UIのサイズ
	float width_;  //UIの幅
	float height_; //UIの高さ

	/// <summary>
	/// 円ゲージを描画
	/// </summary>
	/// <param name="progress">ゲージの進行度</param>
	void DrawCircleGauge(const float progress);

	/// <summary>
	/// 長方形ゲージを描画
	/// </summary>
	/// <param name="progress">ゲージの進行度</param>
	void DrawRectGauge(const float progress);

	/// <summary>
	/// 画像の読み込み処理
	/// </summary>
	void LoadImages(void);

	/// <summary>
	/// 四角形ポリゴンを作成して描画する
	/// </summary>
	/// <param name="drawWidth">描画する幅</param>
	/// <param name="texHandle">画像ハンドル</param>
	void MakeQuad(const float drawWidth, const int texHandle);
};

