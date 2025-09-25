#pragma once
#include <memory>
#include <vector>
#include <map>
#include <functional>
#include "../Order/Order.h"
#include "../ActorBase.h"

class StageObject;
class Furnitures;
class Player;
class AnimationController;
class ModelMaterial;
class ModelRenderer;
class IconUI;

class StageManager : public ActorBase
{

public:

	//アニメーション種別(レジ用)
	enum class ANIM_TYPE
	{
		CREATE,
		IDLE,
		PAYING,
	};
	
	//コンストラクタ
	StageManager(Player& player);

	//デストラクタ
	~StageManager(void);

	/// <summary>
	/// 初期化処理
	/// </summary>
	void Init(void) override;

	/// <summary>
	/// 更新処理
	/// </summary>
	void Update(void) override;

	/// <summary>
	/// 描画処理
	/// </summary>
	/// <param name=""></param>
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
	const bool& IsServed(void) const { return isServed_; }

	/// <summary>
	/// 提供データをリセットする
	/// </summary>
	/// <param name=""></param>
	void ResetServeData(void);

	/// <summary>
	/// テーブルの数を取得する
	/// </summary>
	/// <param name=""></param>
	/// <returns>生成しているテーブルの数</returns>
	int GetTableNum(void) const { return static_cast<int>(tables_.size()); }

	/// <summary>
	/// カウンターのモデル情報を取得する
	/// </summary>
	/// <returns>カウンターのモデル情報</returns>
	const Transform& GetCounterTran(void) const;

	/// <summary>
	/// テーブルのモデル情報を取得する
	/// </summary>
	/// <returnsテーブルのモデル情報</returns>
	const Transform& GetTableTran(int index) const;

	/// <summary>
	/// ショーケースのモデル情報を取得する
	/// </summary>
	/// <returns>ショーケースのモデル情報</returns>
	const Transform& GetShowCase(void) const;

	/// <summary>
	/// ゴミ箱のモデル情報を取得する
	/// </summary>
	/// <returns>ゴミ箱のモデル情報</returns>
	const Transform& GetDustBox(void) const;

	/// <summary>
	/// 床のモデル情報を取得する
	/// </summary>
	/// <returns>床のモデル情報</returns>
	const Transform& GetFloorTran(void) const;
			 
private:

	//アニメーション
	std::unique_ptr<AnimationController> animationController_;

	//スイーツケース用トランスフォーム
	Transform caseTran_;
	//レジ用トランスフォーム
	Transform registerTran_;
	//ゴミ箱用トランスフォーム
	Transform dustBoxTran_;

	//プレイヤーの参照
	Player& player_;

	//ステージに配置するオブジェクト達
	std::vector<std::unique_ptr<StageObject>> objects_;

	//ステージに配置する机
	std::vector<std::unique_ptr<StageObject>> tables_;

	//カウンター用
	std::unique_ptr<StageObject> counter_;

	//家具用
	std::unique_ptr<Furnitures> furnitures_;

	//注文された分を用意したかどうか
	bool isServed_;

	//提供済み商品
	Order::OrderData servedItems_;

	//現在のお客の注文内容
	Order::OrderData currentOrder_;

	//各注文が提供されたかどうかのフラグ
	std::vector<bool> isServedItems_; 

	/// <summary>
	/// 3Dモデルの初期化
	/// </summary>
	/// <param name="">各種3Dモデルの初期化と登録</param>
	void Init3DModel(void);

	/// <summary>
	/// SEやBGMの初期化
	/// </summary>
	void InitSound(void);

	/// <summary>
	/// アニメーションの初期化
	/// </summary>
	/// <param name="">レジのアニメーション</param>
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
	/// ラックとのインタラクト処理
	/// </summary>
	/// <param name=""></param>
	void CupRackInteract(void);

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
	/// コーヒーを生成する処理
	/// </summary>
	/// <param name="">マシンに設置されているカップに対して行う処理/param>
	void ProduceCoffee(int index);

	/// <summary>
	/// コーヒーを作る処理
	/// </summary>
	/// <param name="index">配列番号（カップをコーヒーに上書きするので）</param>
	/// <param name="pos">生成する座標</param>
	/// <param name="objName">作るコーヒーのパラメータ名（ホットかアイス)</param>
	void MakeCoffee(int index,StageObject& obj,std::string objName);

	/// <summary>
	/// カップにアイスディスペンサーで氷を入れる処理
	/// </summary>
	/// <param name="index">指定するアイスディスペンサーの要素番号</param>
	void DispenseIce2Cup(int index);

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
};
