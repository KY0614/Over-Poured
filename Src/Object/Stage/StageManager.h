#pragma once
#include <memory>
#include <map>
#include "../ActorBase.h"
#include "../../Common/Vector2.h"

class ResourceManager;
class StageObject;
class Player;

class StageManager : public ActorBase
{

public:
	//オブジェクト関連

	//テーブル関連
	static constexpr int TABLE_NUM = 4;			//テーブルの数

	static constexpr float TABLE_WIDTH = 95.0f;	//テーブルの横幅

	//座標

	static constexpr VECTOR TABLE_POS = { -140.0f, 0.0f, -175.0f };	//テーブルの座標
	static constexpr VECTOR COLUMN_TABLE_POS = { -222.0f, 0.0f, -98.0f };	//列テーブルの座標

	static constexpr VECTOR MACHINE_POS = { -128.0f, 76.0f, -175.0f };	//コーヒーマシンの座標
	static constexpr VECTOR CUPHOT_POS = { -45.0f, 76.0f, -175.0f };	//ホット用カップの座標
	static constexpr VECTOR CUPICE_POS = { 45.0f, 76.0f, -175.0f };		//アイス用カップの座標
	static constexpr VECTOR ICEDIS_POS = { 133.0f, 76.0f, -175.0f };	//アイスディスペンサーの座標
	static constexpr VECTOR LIBS_POS = { 74.0f, 76.0f, -175.0f };		//カップの蓋の座標
	static constexpr VECTOR DUSTBOX_POS = { 227.0f, 0.0f, -175.0f };	//ゴミ箱の座標
	
	//コンストラクタ
	StageManager(Player& player);

	//デストラクタ
	~StageManager(void);

	void Init(void) override;
	void Update(void) override;
	void Draw(void) override;

	/// <summary>
	/// ホットコーヒーを作る
	/// </summary>
	/// <param name="">マシンとホット用カップだけ処理し、カップの場所にコーヒー生成</param>
	void MakeHotCoffee(void);

	/// <summary>
	/// コーヒーに蓋をつける
	/// </summary>
	/// <param name=""></param>
	void LidFollowCup(void);

	void SurveItem(void);

private:

	//プレイヤーの参照
	Player& player_;

	//ステージに配置するオブジェクト達
	std::vector<std::unique_ptr<StageObject>> objects_;

	//ステージに配置する机
	std::vector<std::unique_ptr<StageObject>> tables_;

	std::vector<std::vector<std::unique_ptr<StageObject>>> grid_;

	bool IsInBounds(int x, int y) const;

	void UpdateDebugImGui(void);
};
