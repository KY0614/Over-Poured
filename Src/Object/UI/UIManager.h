#pragma once
#include <vector>
#include <memory>
#include <map>

class OrderUI;
class GaugeUI;
class IconUI;

class UIManager
{
public:
	//インスタンスの生成
	static void CreateInstance(void);

	//インスタンスの取得
	static UIManager& GetInstance(void);

	UIManager(void);
	~UIManager(void);

	void Init(void);
	void Update(void);
	void Draw(void);
	void Release(void);

	//リソースの破棄
	void Destroy(void);

	void AddGaugeUI(GaugeUI* ui);
	void AddOrderUI(OrderUI* ui);
	void AddIconUI(IconUI* ui);

private:
	//シングルトン用インスタンス
	static UIManager* instance_;

	std::vector<GaugeUI*> gaugeUIs_;
	std::vector<OrderUI*> orderUIs_;
	std::vector<IconUI*> iconUIs_;
};

