#pragma once
#include "../StageObject.h"

class GaugeUI;
class IconUI;

class IceDispenser : public StageObject
{
public:

	//氷を生成するまでの時間
	static constexpr float ICE_PRODUCES_TIME = 3.0f;

	/// <summary>
	/// コンストラクタ
	/// </summary>
	/// <param name="objId">オブジェクトID</param>
	/// <param name="player">プレイヤーの参照</param>
	/// <param name="objects">ステージに配置されているオブジェクト</param>
	IceDispenser(const std::string& objId,
		Player& player,
		std::vector<std::unique_ptr<StageObject>>& objects);
	//デストラクタ
	~IceDispenser(void) override = default;

	/// <summary>
	/// 初期化処理
	/// </summary>
	/// <param name="pos">生成する座標</param>
	/// <param name="rotY">回転角（Y軸）</param>
	/// <param name="scale">大きさ</param>
	void Init(const VECTOR pos,
		const float rotY = 0.0f,
		const VECTOR scale = { 1.0f,1.0f,1.0f })override;

	/// <summary>
	/// インタラクト処理
	/// </summary>
	/// <param name="objId">インタラクトするオブジェクトid</param>
	void Interact(const std::string& objId) override;

private:

	//オブジェクトの参照
	std::vector<std::unique_ptr<StageObject>>& objects_;

	//ゲージのUI
	std::shared_ptr<GaugeUI> gaugeUI_;
	//アイコンのUI
	std::shared_ptr<IconUI> iconUI_;


	/// <summary>
	/// 非稼働時の更新処理
	/// </summary>
	void UpdateInActive(void)override;

	/// <summary>
	/// 稼働時の更新処理
	/// </summary>
	void UpdateActive(void)override;

};

