#pragma once
#include "../StageObject.h"

class GaugeUI;
class IconUI;

class Machine : public StageObject
{
public:
	//コーヒーを作るのにかかる時間
	static constexpr float COFFEE_PRODUCES_TIME = 3.0f;

	//UIのオフセット
	static constexpr float UI_OFFSET_Y = 160.0f;

	//コンストラクタ
	Machine(const std::string objId,Player& player,
		std::vector<std::unique_ptr<StageObject>>& object);
	//デストラクタ
	~Machine(void) = default;

	/// <summary>
	/// 初期化処理
	/// </summary>
	/// <param name="pos">生成する座標</param>
	/// <param name="rotY">回転角（Y軸）</param>
	/// <param name="scale">大きさ</param>
	void Init(VECTOR pos, float rotY = 0.0f, VECTOR scale = { 1.0f,1.0f,1.0f }) override;

	/// <summary>
	/// 描画処理
	/// </summary>
	void Draw(void) override;

	/// <summary>
	/// インタラクト処理
	/// </summary>
	/// <param name="objId">インタラクトするオブジェクトid</param>
	void Interact(const std::string& objId) override;

private:

	//オブジェクトの参照
	std::vector<std::unique_ptr<StageObject>>& objects_;

	//ゲージのUI
	std::unique_ptr<GaugeUI> gaugeUI_;

	//アイコンのUI
	std::unique_ptr<IconUI> iconUI_;	//コーヒーを入れるアイコン
	std::unique_ptr<IconUI> hotIconUI_;	//ホットコーヒーのアイコン
	std::unique_ptr<IconUI> iceIconUI_;	//アイスコーヒーのアイコン

	/// <summary>
	/// 非稼働時の更新処理
	/// </summary>
	void UpdateInActive(void)override;

	/// <summary>
	/// 稼働時の更新処理
	/// </summary>
	void UpdateActive(void)override;

};

