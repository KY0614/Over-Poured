//#pragma once


class OrderManager
{
public:

	//インスタンスの生成
	static void CreateInstance(void);

	//インスタンスの取得
	static OrderManager& GetInstance(void);

	void Init(void);
	void Update(void);

	//リソースの破棄
	void Destroy(void);

private:
	//シングルトン用インスタンス
	static OrderManager* instance_;

};

