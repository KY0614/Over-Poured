#pragma once
#include <memory>
#include <vector>
#include <map>
#include <functional>
#include "../Order/Order.h"
#include "../ActorBase.h"

class StageObject;
class Player;
class Interact2D;
class AnimationController;

class StageManager : public ActorBase
{

public:
	//オブジェクト関連

	//テーブル関連
	static constexpr int TABLE_COLUMN_NUM = 3;		//テーブルの数
	static constexpr int TABLE_ROW_FRONT_NUM = 5;	//テーブルの数
	static constexpr int TABLE_ROW_BACK_NUM = 2;	//テーブルの数

	static constexpr float TABLE_WIDTH = 95.0f;	//テーブルの横幅

	//座標

	static constexpr VECTOR TABLE_POS_BACK = { -140.0f, 0.0f, -175.0f };	//テーブルの座標
	static constexpr VECTOR TABLE_POS_FRONT = { -115.0f, 0.0f, 190.0f };	//テーブルの座標
	static constexpr VECTOR COLUMN_TABLE_POS = { -222.0f, 0.0f, -98.0f };	//列テーブルの座標
	static constexpr VECTOR COUNTER_POS = { 225.0f, 0.0f, 190.0f };			//カウンターの座標
	static constexpr VECTOR DUST_BOX_POS = { 320.0f, 0.0f, -173.0f };		//カウンターの座標

	static constexpr VECTOR MACHINE_POS = { -128.0f, 76.0f, -175.0f };	//コーヒーマシンの座標
	static constexpr VECTOR CUPHOT_POS = { -45.0f, 76.0f, -175.0f };	//ホット用カップの座標
	static constexpr VECTOR CUPICE_POS = { 45.0f, 76.0f, -175.0f };		//アイス用カップの座標
	static constexpr VECTOR ICEDIS_POS = { 133.0f, 76.0f, -175.0f };	//アイスディスペンサーの座標
	static constexpr VECTOR LIBS_POS = { 74.0f, 76.0f, -175.0f };		//カップの蓋の座標
	static constexpr VECTOR DUSTBOX_POS = { 227.0f, 0.0f, -175.0f };	//ゴミ箱の座標

	//アニメーション種別
	enum class ANIM_TYPE
	{
		CREATE,
		IDLE,
		PAYING,
		
	};

	enum class MODE {
		GAME_3D,		// 3Dゲームモード
		MACHINE_2D,		// 2Dマシンモード
		ICE_2D,			// 2D製氷機モード
		LIDRACK_2D,		// 2Dリッドラックモード
	};
	
	//コンストラクタ
	StageManager(Player& player);

	//デストラクタ
	~StageManager(void);

	void Init(void) override;
	void Update(void) override;
	void Draw(void) override;

	/// <summary>
	/// 現在の先頭のお客の注文を設定する
	/// </summary>
	/// <param name="order">先頭のお客の注文</param>
	void SetCurrentOrder(const Order::OrderData& order);

	/// <summary>
	/// 提供するアイテムを取得する
	/// </summary>
	/// <param name=""></param>
	/// <returns></returns>
	Order::OrderData GetServeItems(void)const { return servedItems_; }

	/// <summary>
	/// 提供したかどうかを取得する
	/// </summary>
	/// <param name=""></param>
	/// <returns>true:提供した　flase:未提供</returns>
	bool IsServed(void) const { return isServed_; }

	/// <summary>
	/// 提供データをリセットする
	/// </summary>
	/// <param name=""></param>
	void ResetServeData(void);

private:

	//アニメーション
	std::unique_ptr<AnimationController> animationController_;

	//モード管理
	MODE mode_;

	//状態管理(状態遷移時初期処理)
	std::map<MODE, std::function<void(void)>> stateChanges_;

	//状態管理
	std::map<MODE, std::function<void(void)>> updateFunc_;
	std::map<MODE, std::function<void(void)>> drawFunc_;

	//状態管理(更新ステップ)
	std::function<void(void)> modeUpdate_;
	std::function<void(void)> modeDraw_;

	//プレイヤーの参照
	Player& player_;

	//ステージに配置するオブジェクト達
	std::vector<std::unique_ptr<StageObject>> objects_;

	//ステージに配置する机
	std::vector<std::unique_ptr<StageObject>> tables_;

	//カウンター用
	std::unique_ptr<StageObject> counter_;

	std::unique_ptr<Interact2D> interact2D_;	//2Dインタラクト用

	bool isServed_;

	//提供済み商品
	Order::OrderData servedItems_;

	//現在のお客の注文内容
	Order::OrderData currentOrder_;

	//各注文が提供されたかどうかのフラグ
	std::vector<bool> isServedItems_; 

	void InitAnimation(void);

	/// <summary>
	/// 提供するアイテムを設定する
	/// </summary>
	/// <param name="obj"></param>
	void SurveItem(StageObject& obj);

	/// <summary>
	/// 提供したアイテムを削除する
	/// </summary>
	/// <param name=""></param>
	void DeleteSurvedItem(void);

	/// <summary>
	/// 持ち運び可能なオブジェクトのインタラクト処理
	/// </summary>
	/// <param name=""></param>
	void CarryableObjInteract(void);

	/// <summary>
	/// マシンのインタラクト処理
	/// </summary>
	/// <param name="">コーヒーマシンとアイスディスペンサーのインタラクト処理</param>
	void MachineInteract(void);

	/// <summary>
	/// 蓋のラックのインタラクト処理
	/// </summary>
	/// <param name="">インタラクト中はプレイヤーは操作できない</param>
	void LidRackInteract(void);

	/// <summary>
	/// ホットコーヒーを作る
	/// </summary>
	/// <param name="">マシンとホット用カップだけ処理し、カップの場所にコーヒー生成</param>
	void MakeCoffee(void);
	void MakeHotCoffee(int i);
	void MakeIceCoffee(int i);

	/// <summary>
	///　カップにアイスディスペンサーで氷を入れる処理
	/// </summary>
	/// <param name="">dynamic_castでアイスカップの関数を使用</param>
	void DispenseIce2Cup(void);

	/// <summary>
	/// コーヒーに蓋をつける
	/// </summary>
	/// <param name=""></param>
	void LidFollowCup(void);

	/// <summary>
	/// ゴミ箱のインタラクト処理
	/// </summary>
	/// <param name=""></param>
	void DustBoxInteract(void);

	/// <summary>
	/// オーダー数に応じた数のアイテムを提供しているかどうか
	/// </summary>
	/// <param name=""></param>
	/// <returns></returns>
	bool IsOrderCompleted(void);

	//状態遷移
	void ChangeMode(MODE mode);
	void ChangeMode3DGame(void);
	void ChangeModeMachine2D(void);
	void ChangeModeIce2D(void);
	void ChangeModeLidRack2D(void);

	//更新ステップ
	void Update3DGame(void);
	void UpdateMachine2D(void);
	void UpdateIce2D(void);
	void UpdateLidRack2D(void);

	//描画ステップ
	void Draw3DGame(void);
	void DrawMachine2D(void);
	void DrawIce2D(void);
	void DrawLidRack2D(void);

	void DrawDebug(void);

	void UpdateDebugImGui(void);

};
