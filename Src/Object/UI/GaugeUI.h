#pragma once
#include "UIBase.h"

class GaugeUI : public UIBase
{
public:
	//デフォルトのUIサイズ
	static constexpr float UI_DEFAULT_WIDTH = 80.0f;	//幅
	static constexpr float UI_DEFAULT_HEIGHT = 10.0f;	//高さ

	//コンストラクタ
    GaugeUI(bool isCircle, float activeTime);
	//デストラクタ
	~GaugeUI(void) = default;

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
	/// 表示時間を設定
	/// </summary>
	/// <param name="time">表示する時間</param>
	void SetActiveTime(const float time) { activeTime_ = time; }

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
	void DrawCircleGauge(float progress);

	/// <summary>
	/// 
	/// </summary>
	/// <param name="progress">ゲージの進行度</param>
	void DrawRectGauge(float progress);
};

