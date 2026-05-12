#pragma once
#include "../StageObject.h"

class Table : public StageObject
{
public:
	/// <summary>
	/// コンストラクタ
	/// </summary>
	/// <param name="objId">オブジェクトID</param>
	/// <param name="player">プレイヤーの参照</param>
	/// <param name="objects">ステージに配置されているオブジェクト</param>
	Table(const std::string& objId,
		Player& player,
		std::vector<std::unique_ptr<StageObject>>& objects);

	//デストラクタ
	~Table(void) override = default;

	/// <summary>
	/// 更新処理
	/// </summary>
	void Update(void)override;

private:
	//オブジェクトの参照
	std::vector<std::unique_ptr<StageObject>>& objects_;
};

