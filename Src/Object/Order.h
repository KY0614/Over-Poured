#pragma once

class Order
{
public:

	//注文関連
	static constexpr int ORDER_MAX_NUM = 2;			//最大注文内容数

	static constexpr float ONE_ORDER_TIME = 3.5f;	//注文の制限時間(1つ)
	static constexpr float TWO_ORDER_TIME = 5.5f;	//注文の制限時間(２つ)

	//商品関連
	static constexpr int DRINK_MAX_NUM = 2;			//飲み物類の最大数
	static constexpr int FOOD_MAX_NUM = 2;			//食べ物類の最大数

	//ドリンクの種類
	enum class DRINK
	{
		NONE,
		HOT,	//ホット
		ICE,	//アイス
	};

	//スイーツの種類
	enum class SWEETS
	{
		NONE,		//無し
		CHOCO,		//チョコ
		STRAWBERRY,	//ストロベリー
	};

	//注文の内容
	struct OrderData
	{
		int num_;			//注文数
		DRINK drink_;		//飲み物
		SWEETS sweets_;		//スイーツ
		float time_;		//注文の制限時間
	};

	//コンストラクタ
	Order(void);

	//デストラクタ
	~Order(void);

	void Init(void);
	void Update(void);
	void Draw(void);

	//注文を生成する
	void CreateOrder(void);		//１つ生成

	//設定する関数-------------------------------------------------

	/// <summary>
	/// 注文数を設定する
	/// </summary>
	/// <param name="num"></param>
	void SetOrderNum(int num) { orderData_.num_ = num; }

	/// <summary>
	/// 注文内容の飲み物を設定する
	/// </summary>
	/// <param name="type">設定するもの</param>
	void SetDrink(DRINK type) { orderData_.drink_ = type; }

	/// <summary>
	/// 注文内容の食べ物を設定する
	/// </summary>
	/// <param name="type">設定するもの</param>
	void SetSweets(SWEETS type) { orderData_.sweets_ = type; };

	/// <summary>
	/// 注文の制限時間を設定する
	/// </summary>
	/// <param name="time">設定する時間</param>
	void SetOrderTime(float time) { orderData_.time_ = time; }

	//--------------------------------------------------------------

	//取得する関数--------------------------------------------------------------------

	int GetOrderNum(void) { return orderNum_; }		//注文数を取得する

	OrderData GetOrder(void) { return orderData_; }			//注文内容を取得する

	float GetOrderTime(void) { return orderData_.time_; }	//注文の制限時間を取得する

private:
	//注文内容
	OrderData orderData_;

	//注文数
	int orderNum_;

	//void UpdateImgGUI(void);
};

