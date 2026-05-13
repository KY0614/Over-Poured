#pragma once
#include <vector>
#include "UIBase.h"
#include "../Order/Order.h"

class OrderUI :  public UIBase
{
public:
    //オーダーの数
	static constexpr int ORDER_NUM = 2; 

	//背景UIのサイズ
	static constexpr float BACK_IMG_SIZE = 300.0f;  //背景画像のサイズ
	static constexpr float IMG_SIZE = 100.0f;       //ドリンクとスイーツの画像サイズ
	static constexpr float UI_IMG_CENTER = 0.5f;    //UI画像の中心座標(1.0が画像の端、0.5が画像の中心、0.0が画像の反対の端)

    //注文データ
    struct OrderUIData
    {
		Order::DRINK drinkType_ = Order::DRINK::NONE;        //ドリンクの種類
		Order::SWEETS sweetsType_ = Order::SWEETS::NONE;  	//スイーツの種類

		int drinkUIImg_ = -1;           //ドリンクのUI画像
		int sweetsUIImg_ = -1;          //スイーツのUI画像
		int backUIImg_ = -1;            //背景のUI画像

		int timerBackImg_ = -1;         //タイマーの背景画像
		int timerImg_ = -1;             //タイマーの画像

		int checkImg_ = -1;             //チェックの画像

		float currentRate_ = 0.0f;	    //現在のゲージの割合
		float displayedRate_ = 0.0f;    //表示されているゲージの割合
    };

    /// <summary>
    /// コンストラクタ
    /// </summary>
    /// <param name="drink">ドリンクの種類</param>
    /// <param name="sweets">スイーツの種類</param>
    /// <param name="maxTime">注文の制限時間</param>
    OrderUI(const Order::DRINK drink,
        const Order::SWEETS sweets,
        const float maxTime);
	//デストラクタ
    ~OrderUI() override = default;

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
    /// 注文の残り時間を設定
    /// </summary>
    /// <param name="progressT">経過時間</param>
    void SetOrderTimer(const float progressT) { orderTimer_ = progressT; }

	/// <summary>
	/// チェックマークの状態を設定
	/// </summary>
	/// <param name="index">指定する配列</param>
	/// <param name="isCheck">true:表示　false:非表示</param>
	void SetCheckUI(const int index, const bool isActive) { isOrderCheck_[index] = isActive; }

private:

	//オーダーのUIデータ
    OrderUIData orderUIData_;

	//提供された商品がオーダーと一致しているかどうかのチェック
    std::vector<bool> isOrderCheck_;

    //ゲージを進ませる用の時間
    float gaugeTime_;

	//ゲージの最大時間
    float orderMaxTime_;
	//ゲージの現在の時間
    float orderTimer_;

	//注文内容画像のサイズ
    float size_;

    /// <summary>
    /// 画像の読み込み処理
    /// </summary>
    void LoadImages(void);

	/// <summary>
	/// サイズの設定
	/// </summary>
	/// <param name="size">サイズ</param>
	void SetSize(const float size) { size_ = size; }   

	/// <summary>
	/// 注文内容のUIを描画する処理
	/// </summary>
	void DrawOrderUI(void);

    /// <summary>
	/// タイマーUIを描画する処理
    /// </summary>
    void DrawTimerUI(void);
};

