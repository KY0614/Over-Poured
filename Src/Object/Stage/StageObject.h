#pragma once
#include <string>
#include <functional>
#include "../ActorBase.h"
#include "../Manager/GameSystem/OrderCustomerManager.h"
#include "StageObjectLibrary.h"

class Sphere;
class Player;

class StageObject : public ActorBase
{
public:
	//マシンのY座標オフセット(マシンの上に置くようにしたいため)
	static constexpr float MACHINE_OFSET_Z = 15.0f;	
	static constexpr float DISPENSER_OFSET_Z = 17.0f;	
	static constexpr float MACHINE_OFSET_Y = 16.0f;	

	//ラックの最大在庫数
	static constexpr int SWEETS_STOCK_MAX = 4;	//スイーツのラックの最大在庫数
	static constexpr int CUP_STOCK_MAX = 5;		//カップのラックの最大在庫数

	//１つずつ追加（補充）する間隔
	static constexpr float ADD_INTERVAL = 0.7f;		

	//オブジェクトの当たり判定用球体の半径
	static constexpr float SWEETS_HALF_WIDTH = 18.0f;
	static constexpr float SWEETS_HEIGHT_OFFSET = 12.0f;
	static constexpr float SWEETS_Z_FRONT_OFFSET = 2.0f;
	static constexpr float SWEETS_Z_BACK_OFFSET = -19.0f;
	static constexpr float SWEETS_ROT_X = 40.0f;

	static constexpr float CUPS_HALF_WIDTH = 15.0f;
	static constexpr float CUPS_HEIGHT_OFFSET = 35.0f;
	static constexpr float CUPS_Z_OFFSET = 0.0f;

	//オブジェクトID
	static constexpr const char* HOT_COFFEE = "Hot_Coffee";					//ホットコーヒー
	static constexpr const char* ICE_COFFEE = "Ice_Coffee";					//アイスコーヒー
	static constexpr const char* HOT_CUP = "Hot_Cup";						//ホット用カップ
	static constexpr const char* ICE_CUP = "Ice_Cup";						//アイス用カップ
	static constexpr const char* CUP_WITH_ICE = "Cup_With_Ice";				//氷入りのアイス用カップ
	static constexpr const char* TABLE = "Table";							//テーブル
	static constexpr const char* COUNTER = "Counter";						//カウンター
	static constexpr const char* HOT_CUP_RACK = "Cup_Hot_Rack";				//ホットカップ用ラック
	static constexpr const char* ICE_CUP_RACK = "Cup_Ice_Rack";				//アイスカップ用ラック
    static constexpr const char* CHOCO_SWEETSRACK = "Sweets_Choco_Rack";	//チョコスイーツ用ラック
	static constexpr const char* BERRY_SWEETSRACK = "Sweets_Strawberry_Rack";//ベリースイーツ用ラック
	static constexpr const char* CHOCO_SWEETS = "Sweets_Choco";				//チョコスイーツ
	static constexpr const char* BERRY_SWEETS = "Sweets_Strawberry";		//ベリースイーツ
	static constexpr const char* COFFEE_MACHINE = "Coffee_Machine";			//コーヒーマシン
	static constexpr const char* CUP_LID_RACK = "Cup_Lid_Rack";				//蓋のラック
	static constexpr const char* HOTCUP_LID = "Hot_Cup_Lid";				//蓋（ホット）
	static constexpr const char* ICECUP_LID = "Ice_Cup_Lid";				//蓋
	static constexpr const char* ICE_DISPENSER = "Ice_Dispenser";			//製氷機
	static constexpr const char* DUST_BOX = "Dust_Box";						//ゴミ箱

	//アイテムオブジェクトの状態
	enum class ITEM_STATE 
	{
		NONE,
		PLACED,
		HOLD,
	};

	//マシンの稼働状態
	enum class MACHINE_STATE 
	{
		NONE,
		INACTIVE,
		ACTIVE,
	};

	StageObject(const std::string objId,
		const float height,Player& player);

	~StageObject(void);

	virtual void Init(VECTOR pos, float rotY = 0.0f, VECTOR scale = {1.0f,1.0f,1.0f});
	virtual void Update(void)override;
	virtual void Draw(void)override;

	//設定用、変更用関数-------------------------------------------------------------

	/// <summary>
	/// 座標を設定する
	/// </summary>
	/// <param name="pos">設定座標</param>
	void SetPos(VECTOR pos);

	void SetScale(VECTOR scale);

	/// <summary>
	/// 初期の回転を設定する(入れるときはEuler角で設定する)
	/// </summary>
	/// <param name="pos">回転</param>
	void SetQuaRotY(const float localRotY);

	void SetInteractTime(const float time) { param_.interactTime_ = time; }

	void IsNotActioned(void) { isActioned_ = false; }

	void PutOnTheLid(void) { isLid_ = true; }

	void SetLidOn(const bool isLid) { isLid_ = isLid; }

	/// <summary>
	/// アイテムオブジェクトの状態を変更する
	/// </summary>
	/// <param name="state">変更する状態</param>
	void ChangeItemState(ITEM_STATE state) { itemState_ = state; }

	/// <summary>
	/// マシンの状態を変更する
	/// </summary>
	/// <param name="state">変更する状態</param>
	void ChangeMachineState(MACHINE_STATE state) { machineState_ = state; }

	//取得用関数--------------------------------------------------------------------

	/// <summary>
	/// オブジェクトの座標を取得する
	/// </summary>
	/// <param name="">transformの座標を返す</param>
	/// <returns>オブジェクトの座標</returns>
	VECTOR GetPos(void)const { return transform_.pos; };

	/// <summary>
	/// 当たり判定用球体の座標を取得する
	/// </summary>
	/// <returns>球体の座標</returns>
	VECTOR GetSpherePos(void)const;

	float GetObjHeight(void)const { return height_; }

	StageObjectLibrary::ObjectParams GetParam(void)const { return param_; }

	/// <summary>
	/// アイテムオブジェクトの状態を取得
	/// </summary>
	/// <param name="">itemState_を返す</param>
	/// <returns>アイテムオブジェクトの状態</returns>
	ITEM_STATE GetItemState(void)const { return itemState_; }

	/// <summary>
	/// マシンオブジェクトの状態を取得
	/// </summary>
	/// <param name="">machineState_を返す</param>
	/// <returns>マシンオブジェクトの状態</returns>
	MACHINE_STATE GetMachineState(void)const { return machineState_; }

	/// <summary>
	/// モデルの上部中央座標を取得する(テーブル用)
	/// </summary>
	/// <param name="">transformのpos.yにheight_を足して返す</param>
	/// <returns>上部中央座標</returns>
	VECTOR GetTopCenter(void)const;

	/// <summary>
	/// 当たり判定用球体の半径を取得する
	/// </summary>
	/// <param name="">SphereのGetRadius関数を返す</param>
	/// <returns>球体の半径</returns>
	float GetSphereRad(void)const;

	/// <summary>
	/// 
	/// </summary>
	/// <param name=""></param>
	/// <returns></returns>
	bool IsActioned(void) const;

	/// <summary>
	/// 蓋が乗っているかどうか
	/// </summary>
	/// <param name=""></param>
	/// <returns>true:乗っている、false:乗っていない</returns>
	bool IsLidOn(void) const { return isLid_; }

	/// <summary>
	/// ラックに在庫があるかどうか
	/// </summary>
	/// <param name="">在庫がないときはfalseを返す</param>
	/// <returns>ture:ある　false:ない</returns>
	bool GetHasStock(void) const { return hasStock_; }

	Order::DRINK GetDrinkType(void) const { return drink_; }
	Order::SWEETS GetSweetsType(void) const { return sweets_; }

	const Transform& GetTalbeColTran(void) const{ return colTran_; }

	//-------------------------------------------------------------------------

	/// <summary>
	/// オブジェクトを持ち運ぶ処理
	/// </summary>
	/// <param name="">スペースキー押下で持つ</param>
	virtual void ItemCarry(void);

	/// <summary>
	/// オブジェクトを設置する処理
	/// </summary>
	/// <param name="pos">設置する座標</param>
	virtual void ItemPlaced(VECTOR pos);

	/// <summary>
	/// オブジェクトに対してインタラクトする処理
	/// </summary>
	/// <param name="objId">インタラクトさせたいオブジェクトのID</param>
	virtual void Interact(const std::string& objId);

	/// <summary>
	/// オブジェクトを取り出す処理
	/// </summary>
	/// <param name="object"></param>
	virtual void PickUp(std::string rackName,std::vector<std::unique_ptr<StageObject>>& object);

	virtual void AddStock(int addStockNum);

protected:

	//オブジェクトのパラメータ
	StageObjectLibrary::ObjectParams param_;

	//テーブル用のコライダー
	Transform colTran_;

	//プレイヤーの参照
	Player& player_;

	//仮モデルの立方体
	float height_;	//高さ

	//当たり判定用球体
	std::unique_ptr<Sphere> sphere_;

	//当たり判定用球体の半径
	float collisionRad_;

	//プレイヤーがアクションを行ったかどうか
	bool isActioned_;	

	//蓋をしているかどうか(コーヒー以外のオブジェクトはfalse）
	bool isLid_;

	//ラック用------------------------

	//ラックに在庫があるかどうか
	bool hasStock_;

	//--------------------------------

	Order::DRINK drink_;
	Order::SWEETS sweets_;

	virtual void UpdatePlaced(void);
	virtual void UpdateHold(void);
	
	virtual void UpdateInActive(void);
	virtual void UpdateActive(void);

	std::string objId_;
	std::pair<std::string, StageObjectLibrary::ObjectParams> object_;

	int interactImg_;

	bool isInteract_;

private:
	//アイテムオブジェクトの状態
	ITEM_STATE itemState_;

	//マシンの稼働状態
	MACHINE_STATE machineState_;
};