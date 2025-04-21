//#pragma once

class Order
{
public:

	//コンストラクタ
	Order(void);

	//デストラクタ
	~Order(void);

	void Init(void);
	void Update(void);
	void Draw(void);

	/// <summary>
	/// 注文に対して制限時間を設定する
	/// </summary>
	/// <param name="time">設定する制限時間</param>
	void SetTimer(float time) { timer_ = time; }

	float GetTimer(void) { return timer_; }		//制限時間を取得する

private:

	//制限時間用
	float timer_;

	void UpdateImgGUI(void);
};

