#pragma once
#include <string>
#include <functional>
#include "../ActorBase.h"
#include "StageObjectLibrary.h"

class Cube;
class Sphere;
class Player;

class StageObject : public ActorBase
{
public:

	//アイテムオブジェクトの状態
	enum class ITEM_STATE {
		NONE,
		PLACED,
		HOLD,
	};

	//マシンの稼働状態
	enum class MACHINE_STATE {
		NONE,
		INACTIVE,
		ACTIVE,
	};

	StageObject(const std::string objId,const float width,
		const float height, const float depth,Player& player);

	~StageObject(void);

	virtual void Init(void)override;
	virtual void Update(void)override;
	virtual void Draw(void)override;

	//設定用、変更用関数-------------------------------------------------------------

	/// <summary>
	/// 座標を設定する
	/// </summary>
	/// <param name="pos">設定座標</param>
	void SetPos(VECTOR pos);
	void SetFollowPos(void) { transform_.pos = GetRotPos(follow_.localPos); }

	void SetFollowPos(VECTOR pos) { follow_.pos = pos; }
	void SetFollowLocalPos(VECTOR localPos) { follow_.localPos = localPos; }

	void SetFollow(const Transform& parent) { follow_ = parent; }

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

	/// <summary>
	/// 持ち運び可能のオブジェクトかどうか
	/// </summary>
	/// <param name=""></param>
	/// <returns>true:持ち運び可能、false:不可能</returns>
	bool GetIsCarryable(void)const { return param_.carryable_; }

	/// <summary>
	/// 設置可能オブジェクトかどうか
	/// </summary>
	/// <param name=""></param>
	/// <returns>true:設置可能、false:不可能</returns>
	bool GetIsPlaceable(void)const { return param_.placeable_; }

	/// <summary>
	/// プレイヤーがアクション可能なオブジェクトかどうか
	/// </summary>
	/// <param name=""></param>
	/// <returns>true:アクション可能、false:不可能</returns>
	bool GetIsInteractable(void)const { return param_.interactable_; }

	/// <summary>
	/// インタラクトタイプ（動作タイプ）を取得する
	/// </summary>
	/// <param name="">オブジェクトのパラメータ</param>
	/// <returns>インタラクトタイプ</returns>
	std::string GetInteractType(void)const { return param_.interactType_; }

	/// <summary>
	/// インタラクト時間を取得する
	/// </summary>
	/// <param name="">オブジェクトのパラメータ</param>
	/// <returns>インタラクト時間</returns>
	float GetInteractTime(void)const { return param_.interactTime; }

	/// <summary>
	/// オブジェクトのIDを取得する
	/// </summary>
	/// <param name="">オブジェクトのパラメータ</param>
	/// <returns>ID</returns>
	std::string GetObjectId(void)const { return param_.id_; }

	std::string GetObjCategory(void)const { return param_.category_; }

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
	virtual void PickUp(std::vector<std::unique_ptr<StageObject>>& object);

protected:

	//オブジェクトのパラメータ
	StageObjectLibrary::ObjectParams param_;

	Player& player_;

	Transform follow_;

	//仮モデルの立方体
	std::unique_ptr<Cube> cube_;
	float width_;	//横幅
	float height_;	//高さ
	float depth_;	//奥行

	//当たり判定用球体
	std::unique_ptr<Sphere> sphere_;

	float rad_;		//半径

	//
	bool isActioned_;	

	virtual void UpdatePlaced(void);
	virtual void UpdateHold(void);
	
	virtual void UpdateInActive(void);
	virtual void UpdateActive(void);

private:
	std::string objId_;

	std::pair<std::string, StageObjectLibrary::ObjectParams> object_;

	ITEM_STATE itemState_;
	MACHINE_STATE machineState_;

	//相対座標を回転させてワールド座標で取得する
	VECTOR GetRotPos(const VECTOR& localPos) const;

	/// <summary>
	/// 座標や拡大率を調整する用のGUI
	/// </summary>
	/// <param name=""></param>
	void UpdateDebugImGui(void);
};

