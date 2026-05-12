#pragma once
#include "../StageObject.h"

class FollowingObject : public StageObject
{
public:
	/// <summary>
	/// コンストラクタ
	/// </summary>
	/// <param name="objId">オブジェクトID</param>
	/// <param name="player">プレイヤーの参照</param>
	/// <param name="followObject">追従先のオブジェクト</param>
	FollowingObject(const std::string& objId,
		Player& player,
		StageObject& followObject);
	//デストラクタ
	~FollowingObject(void) override = default;

	/// <summary>
	/// 初期化処理
	/// </summary>
	/// <param name="pos">生成する座標</param>
	/// <param name="rotY">回転角（Y軸）</param>
	/// <param name="scale">大きさ</param>
	void Init(const VECTOR pos,
		const float rotY = 0.0f,
		const VECTOR scale = { 1.0f,1.0f,1.0f })override;

	/// <summary>
	/// 更新処理
	/// </summary>
	void Update(void)override;

	/// <summary>
	/// 追従しているオブジェクトを取得
	/// </summary>
	/// <param name=""></param>
	/// <returns>追従元のオブジェクト</returns>
	const StageObject& GetFollowedObj(void) const { return followObj_; }

private:
	//追従元のオブジェクト
	StageObject& followObj_;
};

