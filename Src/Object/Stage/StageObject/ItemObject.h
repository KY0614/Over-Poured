#pragma once
#include "../StageObject.h"

class ItemObject : public StageObject
{
public:
	//コンストラクタ
	ItemObject(const std::string objId, Player& player);
	//デストラクタ
	~ItemObject(void) = default;

	/// <summary>
	/// アイテムを持ち運ぶ処理
	/// </summary>
	void ItemCarry(void) override;

	/// <summary>
	/// アイテムを設置する処理
	/// </summary>
	/// <param name="pos">設置する座標</param>
	void ItemPlaced(VECTOR pos)override;

	/// <summary>
	/// 氷を入れる処理
	/// </summary>
	/// <param name=""></param>
	void PouredIce(void);

	/// <summary>
	/// 氷が入っているかどうかを取得する
	/// </summary>
	/// <param name=""></param>
	/// <returns>氷入りかどうか</returns>
	bool IsIce(void)const { return isIced_; }

	/// <summary>
	/// 描画処理
	/// </summary>
	void Draw(void) override;

private:

	//氷が入っているかどうか
	bool isIced_;

	/// <summary>
	/// 設置状態の更新処理
	/// </summary>
	void UpdatePlaced(void)override;

	/// <summary>
	/// 所持状態の更新処理
	/// </summary>
	void UpdateHold(void)override;
};

