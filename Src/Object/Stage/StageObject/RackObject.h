#pragma once
#include "../StageObject.h"

class GaugeUI;
class IconUI;

class RackObject : public StageObject
{
public:
	//UIのオフセット位置
	static constexpr float SWEETS_UI_OFFSET_Y = 110.0f;	//スイーツ用
	static constexpr float CUP_UI_OFFSET_Y = 30.0f;		//カップ用

	//コンストラクタ
	RackObject(const std::string objId,Player& player);
	//デストラクタ
	~RackObject(void) = default;

	/// <summary>
	/// ラックからオブジェクトを取る処理
	/// </summary>
	/// <param name="rackName">オブジェクトのId</param>
	/// <param name="object">オブジェクトのリスト</param>
	void PickUp(std::string rackName,std::vector<std::unique_ptr<StageObject>>& object) override;

	/// <summary>
	/// /在庫を追加する処理
	/// </summary>
	void AddStock(void) override;

	/// <summary>
	/// 初期化処理
	/// </summary>
	/// <param name="pos">生成する座標</param>
	/// <param name="rotY">回転角（Y軸）</param>
	/// <param name="scale">大きさ</param>
	void Init(VECTOR pos, float rotY = 0.0f, VECTOR scale = { 1.0f,1.0f,1.0f }) override;

	/// <summary>
	/// 更新処理
	/// </summary>
	void Update(void) override;

	/// <summary>
	/// 描画処理
	/// </summary>
	void Draw(void) override;

private:

	//オブジェクトの残り個数
	int sweetsStockCnt_;	//スイーツ用
	int cupsStockCnt_;		//カップ用

	//追加される間隔
	float addInterval_;	

	//ゲージのUI
	std::unique_ptr<GaugeUI> gaugeUI_;

	//アイコンUI
	std::unique_ptr<IconUI> iconUI_;
	
	//在庫がないアイコンUI
	std::unique_ptr<IconUI> stockIconUI_;

	//ラックに置かれるスイーツとカップのTransform
	Transform sweetsOfRack_[SWEETS_STOCK_MAX];	//スイーツ用
	Transform cupesOfRack_[CUP_STOCK_MAX];		//カップ用
};

