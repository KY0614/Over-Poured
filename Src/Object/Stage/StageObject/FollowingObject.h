#pragma once
#include "../StageObject.h"

class FollowingObject : public StageObject
{
public:
	//コンストラクタ
	FollowingObject(const std::string objId,Player& player,
		StageObject& followObject);
	//デストラクタ
	~FollowingObject(void) = default;

	/// <summary>
	/// 初期化処理
	/// </summary>
	/// <param name="pos">生成する座標</param>
	/// <param name="rotY">回転角（Y軸）</param>
	/// <param name="scale">大きさ</param>
	void Init(VECTOR pos, float rotY = 0.0f, VECTOR scale = { 1.0f,1.0f,1.0f })override;

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

