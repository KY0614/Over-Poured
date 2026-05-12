#pragma once
#include "../StageObject.h"

class DustBox : public StageObject
{
public:
	/// <summary>
	/// コンストラクタ
	/// </summary>
	/// <param name="objId">オブジェクトID</param>
	/// <param name="player">プレイヤーの参照</param>
	/// <param name="objects">ステージに配置されているオブジェクト</param>
	DustBox(const std::string& objId,
		Player& player,
		std::vector<std::unique_ptr<StageObject>>& objects);

	//デストラクタ
	~DustBox(void) override = default;

	/// <summary>
	/// インタラクト処理
	/// </summary>
	/// <param name="objId">捨てるオブジェクトid</param>
	void Interact(const std::string& objId) override;

private:
	//オブジェクトの参照
	std::vector<std::unique_ptr<StageObject>>& objects_;
};

