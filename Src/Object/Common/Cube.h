#pragma once
#include <DxLib.h>

class Transform;

class Cube
{
public:
	//最大頂点数
	static constexpr int VERTEX_NUM = 36;	

	//デバッグ時の簡易カプセル表示の色
	static constexpr int COLOR = 0xffffff;

	//コンストラクタ
	//カプセルをつけるオブジェクトを指定する
	Cube(const Transform& parent);

	//デストラクタ
	~Cube(void);

	/// <summary>
	/// 立方体を作成する
	/// </summary>
	/// <param name="center">中心座標</param>
	/// <param name="size">サイズ</param>
	/// <param name="col">色</param>
	void MakeCube(VECTOR center, float size, COLOR_U8  col);

	/// <summary>
	/// 立方体を作成する
	/// </summary>
	/// <param name="center">生成する座標(立方体の中心座標）</param>
	/// <param name="width">幅</param>
	/// <param name="height">高さ</param>
	/// <param name="depth">奥行き</param>
	/// <param name="col">色</param>
	void MakeBox(VECTOR center, float width, float height, float depth, COLOR_U8  col);

private:
	// 頂点配列（最大12枚の三角形 → 36頂点）
	VERTEX3D Vertex[VERTEX_NUM];

	//カプセルをくっつける相手
	const Transform& transformParent_;

	//親Transformからの相対位置(上側)
	VECTOR localPosTop_;

	//親Transformからの相対位置(下側)
	VECTOR localPosDown_;
};

