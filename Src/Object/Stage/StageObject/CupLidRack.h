#pragma once
#include "../StageObject.h"

class GaugeUI;

class CupLidRack : public StageObject
{
public:
	//蓋するまでの時間
	static constexpr float LID_PRODUCES_TIME = 3.0f;

	/// <summary>
	/// コンストラクタ
	/// </summary>
	/// <param name="objId">オブジェクトID</param>
	/// <param name="player">プレイヤーの参照</param>
	/// <param name="objects">ステージに配置されているオブジェクト</param>
	CupLidRack(const std::string& objId, 
		Player& player,
		std::vector<std::unique_ptr<StageObject>>& objects);
	//デストラクタ
	~CupLidRack(void) override = default;

	/// <summary>
	/// インタラクト処理
	/// </summary>
	/// <param name="objId">捨てるオブジェクトid</param>
	void Interact(const std::string& objId) override;

	/// <summary>
	/// 初期化処理
	/// </summary>
	/// <param name="pos">生成する座標</param>
	/// <param name="rotY">回転角（Y軸）</param>
	/// <param name="scale">大きさ</param>
	void Init(VECTOR pos, float rotY = 0.0f, VECTOR scale = { 1.0f,1.0f,1.0f })override;

	/// <summary>
	/// 更新処理
	/// </summary>
	void Update(void)override;

private:
	//オブジェクトの参照
	std::vector<std::unique_ptr<StageObject>>& objects_;

	//ゲージのUI
	std::shared_ptr<GaugeUI> gaugeUI_;
};

