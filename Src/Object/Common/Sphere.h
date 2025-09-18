#pragma once
#include <DxLib.h>

class Transform;

class Sphere
{
public:
	//球体の色
	static constexpr int COLOR = 0x000000;

	//球体の分割数
	static constexpr int DIV_NUM = 5;

	//コンストラクタ
	//球体をつけるオブジェクトを指定する
	Sphere(const Transform& parent);


	//デストラクタ
	~Sphere(void);

	/// <summary>
	/// 描画
	/// </summary>
	void Draw(void);

	/// <summary>
	/// 指定した色で球体を描画
	/// </summary>
	/// <param name="col">指定する色</param>
	void Draw(int col);

	/// <summary>
	/// 親Transformからの相対位置を取得
	/// </summary>
	/// <returns>親Transformからの相対座標</returns>
	VECTOR GetLocalPos(void) const { return localPos_; }

	/// <summary>
	/// 親Transformからの相対座標をセット
	/// </summary>
	/// <param name="pos">設定する相対座標</param>
	void SetLocalPos(const VECTOR& pos) { localPos_ = pos; }

	/// <summary>
	/// ワールド座標を取得
	/// </summary>
	/// <returns>相対座標のワールド座標</returns>
	VECTOR GetPos(void) const { return GetRotPos(localPos_); }

	/// <summary>
	/// 相対座標を回転させてワールド座標で取得する
	/// </summary>
	/// <param name="localPos">相対座標</param>
	/// <returns>相対座標のワールド座標</returns>
	VECTOR GetRotPos(const VECTOR& localPos) const;

	/// <summary>
	/// 球体の半径を取得
	/// </summary>
	/// <returns>半径の値</returns>
	float GetRadius(void) const { return radius_; }

	/// <summary>
	/// 半径を設定
	/// </summary>
	/// <param name="radius">設定する半径の値</param>
	void SetRadius(float radius) { radius_ = radius; }

private:

	//球体をくっつける相手
	const Transform& transformParent_;

	//親Transformからの相対位置
	VECTOR localPos_;

	//半径
	float radius_;
};

