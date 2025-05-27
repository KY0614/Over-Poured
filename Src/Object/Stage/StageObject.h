#pragma once
#include <string>
#include <functional>
#include "../ActorBase.h"
#include "StageObjectLibrary.h"

class Cube;
class Player;

class StageObject : public ActorBase
{
public:

	enum class ITEM_STATE {
		NONE,
		PLACED,
		HOLD,
	};

	StageObject(const std::string objId,const float width,
		const float height, const float depth,Player& player);

	~StageObject(void);

	virtual void Init(void)override;
	virtual void Update(void)override;
	virtual void Draw(void)override;

	void SetPos(VECTOR pos);

	void SetFollowPos(VECTOR followPos) { followPos_ = followPos; }

	void ChangeItemState(ITEM_STATE state) { state_ = state; }

	VECTOR GetPos(void)const { return transform_.pos; };
	VECTOR GetSpherePos(void)const { return sphereTran_.pos; };

	/// <summary>
	/// 持ち運び可能のオブジェクトかどうか
	/// </summary>
	/// <param name=""></param>
	/// <returns>true:持ち運び可能、false:不可能</returns>
	bool IsCarryable(void)const { return param_.carryable_; }

	/// <summary>
	/// 設置可能オブジェクトかどうか
	/// </summary>
	/// <param name=""></param>
	/// <returns>true:設置可能、false:不可能</returns>
	bool IsPlaceable(void)const { return param_.placeable_; }

	/// <summary>
	/// プレイヤーがアクション可能なオブジェクトかどうか
	/// </summary>
	/// <param name=""></param>
	/// <returns>true:アクション可能、false:不可能</returns>
	bool IsInteractable(void)const { return param_.interactable_; }

	std::string GetObjectId(void)const { return objId_; }

	ITEM_STATE GetItemState(void)const { return state_; }

	virtual void ItemCarry(void);

	virtual void ItemPlaced(void);

	virtual void Interact(void);

	VECTOR GetTopCenter(void)const;

	float GetSphereRad(void)const { return rad_; }

protected:

	//オブジェクトのパラメータ
	StageObjectLibrary::ObjectParams param_;

	Player& player_;

	VECTOR followPos_;

	//仮モデルの立方体
	std::unique_ptr<Cube> cube_;
	float width_;
	float height_;
	float depth_;

	float rad_;

	virtual void UpdatePlaced(void);
	virtual void UpdateHold(void);

private:
	std::string objId_;

	std::pair<std::string, StageObjectLibrary::ObjectParams> object_;

	ITEM_STATE state_;

	float fillProgress_ = 0.0f;

	/// <summary>
	/// 座標や拡大率を調整する用のGUI
	/// </summary>
	/// <param name=""></param>
	void UpdateDebugImGui(void);
};

