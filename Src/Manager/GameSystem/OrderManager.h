//#pragma once


class OrderManager
{
public:

	//注文関連
	static constexpr int ORDER_MAX_NUM = 2;		//最大注文数

	static constexpr float ONE_ORDER_TIME = 3.5f;	//注文の制限時間(1つ)
	static constexpr float TWO_ORDER_TIME = 5.5f;	//注文の制限時間(２つ)

	//商品関連
	static constexpr int DRINK_MAX_NUM = 2;		//飲み物類の最大数
	static constexpr int FOOD_MAX_NUM = 2;		//食べ物類の最大数

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
	struct Order
	{
		DRINK drink_;
		SWEETS sweets_;
		float orderTime_;
	};

	//インスタンスの生成
	static void CreateInstance(void);

	//インスタンスの取得
	static OrderManager& GetInstance(void);

	void Init(void);
	void Update(void);

	//リソースの破棄
	void Destroy(void);

	//注文を生成
	Order CreateOrder(void);

	//設定する関数-------------------------------------------------

	/// <summary>
	/// 飲み物の内容を設定する
	/// </summary>
	/// <param name="type">設定するもの</param>
	void SetDrink(DRINK type) { order_.drink_ = type; }

	/// <summary>
	/// 食べ物の内容を設定する
	/// </summary>
	/// <param name="type">設定するもの</param>
	void SetSweets(SWEETS type) { order_.sweets_ = type; };

	/// <summary>
	/// 注文の制限時間を設定する
	/// </summary>
	/// <param name="time">設定する時間</param>
	void SetOrderTime(float time) { order_.orderTime_ = time; }

	//--------------------------------------------------------------

	//取得する関数--------------------------------------------------------------------
	
	int GetOrderNum(void) { return orderNum_; }		//注文数を取得する

	Order GetOrder(void) { return order_; }			//注文内容を取得する

	float GetOrderTime(void) { return order_.orderTime_; }	//注文の制限時間を取得する

private:
	//シングルトン用インスタンス
	static OrderManager* instance_;

	Order order_;
	int orderNum_;
};

